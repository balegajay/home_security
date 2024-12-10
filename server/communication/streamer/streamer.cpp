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

};

Streamer::Streamer(int argc, char *argv[]) {
  /* Initialize GStreamer */
  gst_init(&argc, &argv);
  impl_data_ = new Impl();
}

Streamer::~Streamer() {
  delete impl_data_;
}

bool Streamer::Setup(std::string pipeline) {
  /* Create the pipeline*/
  impl_data_->pipeline =
      std::unique_ptr<GstElement, decltype(&gst_object_unref)>(
          gst_parse_launch(pipeline.c_str(), nullptr),
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