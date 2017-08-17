#include <chrono>
#include <exception>
#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>
#include <boost/filesystem.hpp>

#include <opencv2/opencv.hpp>

#include <FrameDetector.h>

namespace {
    /// Simple proof-of-fail class to show that FrameDetector throws SIGSEGV on start()
    class AffdexTester : public affdex::ImageListener,
                         public affdex::ProcessStatusListener,
                         public affdex::FaceListener {
    public:
        AffdexTester(boost::asio::io_service& ioService)
            : ioService_(ioService)
            , feedTimer_(ioService_)
            , frameDetector_(1)
        {
            std::cout << __func__ << std::endl;
        }

        ~AffdexTester()
        {
            std::cout << __func__ << std::endl;
        }

        void start()
        {
            std::cout << "Setting data path to " << AFFDEX_DATA << std::endl;
            if (!boost::filesystem::exists(AFFDEX_DATA)) {
                throw(std::runtime_error("Affdex data-directory does not exist!"));
            }

            frameDetector_.setClassifierPath(AFFDEX_DATA);

            frameDetector_.setImageListener(this);
            frameDetector_.setProcessStatusListener(this);
            frameDetector_.setFaceListener(this);

            frameDetector_.setDetectAllExpressions(true);
            frameDetector_.setDetectAllEmotions(true);
            frameDetector_.setDetectAllEmojis(true);
            frameDetector_.setDetectAllAppearances(true);

            std::cout << "Starting FrameDetector..." << std::endl;
            // TODO: start() raises a SIGSEGV
            try {
                frameDetector_.start();
            } catch (const std::exception& e) {
                // Will not be catachable (SIGSEGV) but to double check we could'nt intercept anything here...
                std::cerr << "Catchable exception in frameDetector_.start(): " << e.what() << std::endl;
            }
            // TODO: we never reach this point
            std::cout << "...started." << std::endl;

            feedNextFrame();
        }
    private:
        /// This never gets called due to SIGSEGV in frameDetector_.start()
        void feedNextFrame()
        {
            std::cout << "Feeding frame..." << std::endl;

            // TODO: Implement frameDetector_.process(someFrame);

            feedTimer_.expires_from_now(std::chrono::seconds(1));
            auto feedForward = [&](const boost::system::error_code& ec){
                feedNextFrame();
            };

            feedTimer_.async_wait(std::move(feedForward));
        }

        /// FaceListener
        void onFaceFound(float timestamp, affdex::FaceId faceId) override
        {
            std::cout << __func__ << std::endl;
        }

        /// FaceListener
        void onFaceLost(float timestamp, affdex::FaceId faceId) override
        {
            std::cout << __func__ << std::endl;
        }

        /// ImageListener
        void onImageResults(std::map<affdex::FaceId, affdex::Face> faces, affdex::Frame image) override
        {
            std::cout << __func__ << std::endl;
        }

        /// ImageListener
        void onImageCapture(affdex::Frame image) override
        {
            std::cout << __func__ << std::endl;
        }

        /// ProcessStatusListener
        void onProcessingException(affdex::AffdexException ex) override
        {
            std::cout << __func__ << std::endl;
        }

        /// ProcessStatusListener
        void onProcessingFinished() override
        {
            std::cout << __func__ << std::endl;
        }

        boost::asio::io_service& ioService_;
        boost::asio::steady_timer feedTimer_;
        affdex::FrameDetector frameDetector_;
    };
}

int main(int argc, char *argv[])
{
    std::cout << __func__ << std::endl;
    try {
        boost::asio::io_service ioService;

        std::cout << "Constructing AffdexTester..." << std::endl;
        auto tester = std::make_shared<AffdexTester>(ioService);
        std::cout << "Starting AffdexTester..." << std::endl;
        tester->start();

        std::cout << "Running ioService..." << std::endl;
        ioService.run();

        std::cout << "ioService exited." << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
