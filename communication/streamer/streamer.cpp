#include "streamer.hpp"

#include <gst/gst.h>
#include <gst/gstcaps.h>
#include <gst/gstelement.h>
#include <gst/gstmessage.h>
#include <gst/gststructure.h>
#include <gst/gstutils.h>

#include <iostream>
#include <memory>

class Impl {
 public:
  GstElement *pipeline, *source, *rtph, *h264, *decode, *convert, *sink;

  GstBus *bus;
  GMainLoop *loop;

  ~Impl() {
    /* Free resources */
    g_main_loop_unref(loop);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);
  }

  /*This function will be called by the pad-added signal */
  void PadAddedHandler(GstElement *src, GstPad *new_pad, Impl *object) {
    if (!src || !new_pad || !object) {
      std::cout << "pad added handler pointer null" << std::endl;
    }

    if (src == object->decode) {
      gst_element_link(src, object->convert);
      std::cout << "linking element source to element convert successful"
                << std::endl;
    }
  }

  static void PadAddedHandlerCB(GstElement *src, GstPad *new_pad,
                                Impl *object) {
    object->PadAddedHandler(src, new_pad, object);
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

        g_main_loop_quit(loop);
        break;
      case GST_MESSAGE_CLOCK_LOST:
        /* Get a new clock */
        gst_element_set_state(object->pipeline, GST_STATE_PAUSED);
        gst_element_set_state(object->pipeline, GST_STATE_PLAYING);
        break;
      case GST_MESSAGE_EOS:
        std::cout << "here" << std::endl;
        break;
    }
  }

  static void BusMessageHandlerCB(GstBus *bus, GstMessage *msg, Impl *object) {
    if (object) object->BusMessageHandler(bus, msg, object);
  }
};

Streamer::Streamer(int argc, char *argv[], int video_port, int audio_port,
                   std::function<cv::Mat()> call_back)
    : call_back_(call_back), video_port_(video_port), audio_port_(audio_port) {
  /* Initialize GStreamer */
  gst_init(&argc, &argv);
  impl_data_ = new Impl();
}

Streamer::~Streamer() {}

bool Streamer::Setup() {
  /* Create the elements */
  impl_data_->source = gst_element_factory_make("udpsrc", "source");
  impl_data_->rtph = gst_element_factory_make("rtph264depay", "rpth");
  impl_data_->h264 = gst_element_factory_make("h264parse", "h264");
  impl_data_->decode = gst_element_factory_make("decodebin", "decode");
  impl_data_->convert = gst_element_factory_make("videoconvert", "convert");
  impl_data_->sink = gst_element_factory_make("autovideosink", "sink");

  /* Create the empty pipeline */
  impl_data_->pipeline = gst_pipeline_new("pipeline");

  /* Build the pipeline. Note that we are NOT linking the source at this
   * point. We will do it later. */
  gst_bin_add_many(GST_BIN(impl_data_->pipeline), impl_data_->source,
                   impl_data_->rtph, impl_data_->h264, impl_data_->decode,
                   impl_data_->convert, impl_data_->sink, NULL);

  /* Connect to the pad-added signal for linking source to rtph*/
  g_signal_connect(impl_data_->source, "pad-added",
                   G_CALLBACK(impl_data_->PadAddedHandlerCB), impl_data_);

  std::unique_ptr<GstCaps, decltype(&gst_caps_unref)> filter(
      gst_caps_from_string(
          "application/x-rtp, media=(string)video, clock-rate=(int)90000, "
          "encoding-name=(string)H264, payload=(int)96"),
      &gst_caps_unref);

  if (!gst_element_link_filtered(impl_data_->source, impl_data_->rtph,
                                 filter.get())) {
    std::cout << "failed to link element  to element ";
  }
  if (!gst_element_link_many(impl_data_->rtph, impl_data_->h264,
                             impl_data_->decode, NULL))
    std::cout << "could not link rtph->h263->decode";

  if (!gst_element_link(impl_data_->convert, impl_data_->sink))
    std::cout << "could not link convert to sink";

  /* Do not link decode to convert just yet, add the pad-added signal callback
   * for this*/
  g_signal_connect(impl_data_->decode, "pad-added",
                   G_CALLBACK(impl_data_->PadAddedHandlerCB), impl_data_);

  /* Set the property of upd src */
  g_object_set(impl_data_->source, "port", video_port_, NULL);

  /* Set the property of sink*/
  g_object_set(impl_data_->sink, "sync", FALSE, NULL);

  if (!Play()) {
    std::cout << "Start error";
    return false;
  }

  impl_data_->bus = gst_pipeline_get_bus(GST_PIPELINE(impl_data_->pipeline));
  gst_bus_add_signal_watch(impl_data_->bus);
  g_signal_connect(impl_data_->bus, "message",
                   G_CALLBACK(impl_data_->BusMessageHandlerCB), impl_data_);

  return true;
}

bool Streamer::Play() {
  /* Start playing */
  auto ret = gst_element_set_state(impl_data_->pipeline, GST_STATE_PLAYING);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state.\n";
    return false;
  }
  return true;
}

bool Streamer::Pause() {
  /* Stop playing */
  auto ret = gst_element_set_state(impl_data_->pipeline, GST_STATE_PAUSED);
  if (ret == GST_STATE_CHANGE_FAILURE) {
    std::cout << "Unable to set the pipeline to the playing state.\n";
    return false;
  }
  return true;
}

void Streamer::Start() {
  impl_data_->loop = g_main_loop_new(NULL, FALSE);

  g_main_loop_run(impl_data_->loop);
}