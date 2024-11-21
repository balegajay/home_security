
#include <boost/signals2/signal.hpp>
#include <iostream>

#include "IInference.hpp"
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

  // streamer client needs to be created for every streaming server
  Streamer streamer(argc, argv);
  streamer.Setup("gst-launch-1.0 -v udpsrc port=5000 ! application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96 ! rtph264depay ! h264parse ! tee name=t t. ! queue ! splitmuxsink max-size-time=86400000000000 location=recording%02d.mp4 -e t. ! queue leaky=1 ! decodebin ! videoconvert ! autovideosink sync=false");

  // start the streamer
  streamer.Start();

  return 0;
}