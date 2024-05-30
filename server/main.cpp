
#include <boost/signals2/signal.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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

  // create the streamer to get images from the clients
  Streamer streamer(argc, argv, 5000, 1);
  streamer.Setup();

  // connect to the new image signal from the client
  streamer.new_img.connect([/*&inferencer*/](cv::Mat img) {
    // get the model results for the image (model inferencing has a lot of
    // errors)
    // inferencer->Infer(img);
    // TODO use the results in an alert system
  });

  // start the streamer
  streamer.Start();

  return 0;
}