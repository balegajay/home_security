#include "streamer.hpp"

#include <gst/app/gstappsink.h>
#include <gst/gst.h>
#include <gst/gstbus.h>
#include <gst/gstcaps.h>
#include <gst/gstelement.h>
#include <gst/gstmessage.h>
#include <gst/gstobject.h>
#include <gst/gstparse.h>
#include <gst/gststructure.h>
#include <gst/gstutils.h>

#include <iostream>
#include <memory>

class Impl {
 public:
  std::unique_ptr<GstElement, decltype(&gst_object_unref)> pipeline =
      std::unique_ptr<GstElement, decltype(&gst_object_unref)>(
          nullptr, gst_object_unref);

  std::unique_ptr<GstBus, decltype(&gst_object_unref)> bus =
      std::unique_ptr<GstBus, decltype(&gst_object_unref)>(nullptr,
                                                           gst_object_unref);

  std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)> main_loop =
      std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)>(
          nullptr, g_main_loop_unref);

  std::unique_ptr<GstElement, decltype(&gst_object_unref)> sink =
      std::unique_ptr<GstElement, decltype(&gst_object_unref)>(
          nullptr, gst_object_unref);

  boost::signals2::signal<void(cv::Mat)> new_img;

  ~Impl() {
    /* Free resources */
    gst_element_set_state(pipeline.get(), GST_STATE_NULL);
  }

  void BusMessageHandler(GstBus *bus, GstMessage *msg, Impl *object) {
    if (!bus || !msg) return;
    switch (GST_MESSAGE_TYPE(msg)) {
      case GST_MESSAGE_ERROR:
        GError *err;
        gchar *debug;

        gst_message_parse_error(msg, &err, &debug);
        g_print("Error: %s\n", err->message);
        g_error_free(err);
        g_free(debug);
        g_main_loop_quit(object->main_loop.get());
        break;
      case GST_MESSAGE_CLOCK_LOST:
        /* Get a new clock */
        gst_element_set_state(object->pipeline.get(), GST_STATE_PAUSED);
        gst_element_set_state(object->pipeline.get(), GST_STATE_PLAYING);
        break;
      case GST_MESSAGE_EOS:
        std::cout << "end of stream" << std::endl;
        break;
    }
  }

  static void BusMessageHandlerCB(GstBus *bus, GstMessage *msg, Impl *object) {
    if (object) object->BusMessageHandler(bus, msg, object);
  }

  GstFlowReturn NewSampleHandler(GstElement *appsink, Impl *udata) {
    std::unique_ptr<GstSample, decltype(&gst_sample_unref)> sample(
        gst_app_sink_pull_sample((GstAppSink *)appsink), &gst_sample_unref);

    if (!sample) {
      std::cout << "No sample" << std::endl;
      return GST_FLOW_CUSTOM_ERROR;
    }

    auto caps = gst_sample_get_caps(sample.get());
    if (!caps) {
      std::cout << "No caps" << std::endl;
      return GST_FLOW_CUSTOM_ERROR;
    }

    auto structure = gst_caps_get_structure(caps, 0);
    if (!structure) {
      std::cout << "No structure" << std::endl;
      return GST_FLOW_CUSTOM_ERROR;
    }

    int height, width;
    gst_structure_get_int(structure, "width", &width);
    gst_structure_get_int(structure, "height", &height);

    auto sample_buffer = gst_sample_get_buffer(sample.get());
    GstMapInfo map;
    gst_buffer_map(sample_buffer, &map, GST_MAP_READ);
    if (!map.data) std::cout << "errorasdc" << std::endl;

    cv::Mat img(height, width, CV_8UC4, (char *)map.data);
    new_img(img);
    gst_buffer_unmap(sample_buffer, &map);
    return GST_FLOW_OK;
  }

  static GstFlowReturn NewSampleHandlerCB(GstElement *appsink, Impl *udata) {
    return udata->NewSampleHandler(appsink, udata);
  }
};

Streamer::Streamer(int argc, char *argv[], int video_port, int audio_port)
    : video_port_(video_port), audio_port_(audio_port) {
  /* Initialize GStreamer */
  gst_init(&argc, &argv);
  impl_data_ = new Impl();
  impl_data_->new_img.connect(new_img);
}

Streamer::~Streamer() {}

bool Streamer::Setup() {
  /* Create the pipeline*/
  impl_data_->pipeline =
      std::unique_ptr<GstElement, decltype(&gst_object_unref)>(
          gst_parse_launch(
              "gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, "
              "media=(string)video, clock-rate=(int)90000, "
              "encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! "
              "h264parse ! avdec_h264 ! tee name=t ! queue ! videoconvert ! "
              "autovideosink t. ! queue ! videoconvert ! "
              "video/x-raw,format=RGBA ! appsink name=sink",
              nullptr),
          gst_object_unref);

  /* Start playing */
  auto ret =
      gst_element_set_state(impl_data_->pipeline.get(), GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state.\n";
    return false;
  }

  impl_data_->bus = std::unique_ptr<GstBus, decltype(&gst_object_unref)>(
      gst_pipeline_get_bus(GST_PIPELINE(impl_data_->pipeline.get())),
      gst_object_unref);
  gst_bus_add_signal_watch(impl_data_->bus.get());
  g_signal_connect(impl_data_->bus.get(), "message",
                   G_CALLBACK(impl_data_->BusMessageHandlerCB), impl_data_);

  impl_data_->sink = std::unique_ptr<GstElement, decltype(&gst_object_unref)>(
      gst_bin_get_by_name(GST_BIN(impl_data_->pipeline.get()), "sink"),
      gst_object_unref);

  g_object_set(impl_data_->sink.get(), "emit-signals", TRUE, NULL);

  /* connect to signals from sink*/
  g_signal_connect(impl_data_->sink.get(), "new-sample",
                   G_CALLBACK(impl_data_->NewSampleHandlerCB), impl_data_);

  return true;
}

bool Streamer::Pause() {
  /* Stop playing */
  auto ret =
      gst_element_set_state(impl_data_->pipeline.get(), GST_STATE_PAUSED);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state.\n";
    return false;
  }
  return true;
}

void Streamer::Start() {
  impl_data_->main_loop =
      std::unique_ptr<GMainLoop, decltype(&g_main_loop_unref)>(
          g_main_loop_new(NULL, FALSE), g_main_loop_unref);

  g_main_loop_run(impl_data_->main_loop.get());
}