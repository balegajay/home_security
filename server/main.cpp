#include <iostream>
#include "streamer.hpp"
#include "tcp_server.hpp"

int main(int argc, char *argv[]) {
#ifdef APP_VERSION
  std::cout << APP_VERSION << std::endl;
#endif
  boost::asio::io_context sever_context;
  // setup for tcp_server
  TCPServer server(3000, sever_context);
  server.StartAsync();

  // create the inferencer
  // std::unique_ptr<IInference> inferencer =
  //     std::make_unique<TorchInference>("best.torchscript", "");

  // create the streamer to get images from the clients
  std::string pipeline = "gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! h264parse ! decodebin ! videoconvert ! autovideosink sync=false";
  Streamer streamer(argc, argv, 5000, 1);
  streamer.Setup(pipeline);

  // start the streamer
  streamer.Start();

  return 0;
}