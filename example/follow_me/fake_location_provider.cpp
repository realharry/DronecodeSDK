
#include "fake_location_provider.h"
#include <chrono> // for seonds()
#include <thread> // for sleep_for()

using std::this_thread::sleep_for;
using std::chrono::seconds;

FakeLocationProvider::FakeLocationProvider() {}

FakeLocationProvider::~FakeLocationProvider()
{
    stop();
}

void FakeLocationProvider::request_location_updates(location_callback_t callback)
{
    location_callback_ = callback;
    stop();
    start();
}

void FakeLocationProvider::start()
{
    should_exit_ = false;
    thread_ = new std::thread(&FakeLocationProvider::compute_locations, this);
}

void FakeLocationProvider::stop()
{
    should_exit_ = true;

    if (thread_) {
        thread_->join();
        delete thread_;
        thread_ = nullptr;
    }
}

// Rudimentary location provider whose successive lat, lon combination
// makes Drone revolve in a semi-circular path.
void FakeLocationProvider::compute_locations()
{
    while (!should_exit_) {
        if (count_++ < 10) {
            location_callback_(latitude_deg_, longitude_deg_);
            latitude_deg_ -= LATITUDE_DEG_PER_METER * 4;
        }
        if (count_++ < 20) {
            location_callback_(latitude_deg_, longitude_deg_);
            longitude_deg_ += LONGITUDE_DEG_PER_METER * 4;
        }
        if (count_++ < 30) {
            location_callback_(latitude_deg_, longitude_deg_);
            latitude_deg_ += LATITUDE_DEG_PER_METER * 4;
        }
        if (count_++ < 40) {
            location_callback_(latitude_deg_, longitude_deg_);
            longitude_deg_ -= LONGITUDE_DEG_PER_METER * 4;
        }
        if (count_++ < 50) {
            location_callback_(latitude_deg_, longitude_deg_);
            latitude_deg_ -= LATITUDE_DEG_PER_METER * 3;
        }
        if (count_++ < MAX_LOCATIONS) {
            location_callback_(latitude_deg_, longitude_deg_);
            longitude_deg_ += LONGITUDE_DEG_PER_METER * 3;
        }
        sleep_for(seconds(1));
    }
}

const size_t FakeLocationProvider::MAX_LOCATIONS = 60u;
const double FakeLocationProvider::LATITUDE_DEG_PER_METER = 0.000009044;
const double FakeLocationProvider::LONGITUDE_DEG_PER_METER = 0.000008985;
