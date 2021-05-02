#include <iostream>
#include <vector>

using namespace std;

void
svg_begin(double width, double height) {
    cout << "<?xml version='1.0' encoding='UTF-8'?>\n";
    cout << "<svg ";
    cout << "width='" << width << "' ";
    cout << "height='" << height << "' ";
    cout << "viewBox='0 0 " << width << " " << height << "' ";
    cout << "xmlns='http://www.w3.org/2000/svg'>\n";
}

void
svg_end() {
    cout << "</svg>\n";
}

void
svg_text(double left, double baseline, string text) {
    cout << "<text x='" << left << "' y='"<< baseline << "'>"
    << text << "</text>" << endl;

}

void svg_rect(double x, double y, double width, double height, string stroke, string fill){
    cout << "<rect x='" << x <<"' y='" << y << "' width='" << width <<"' height='" << height
    << "' stroke='" << stroke << "' fill='" << fill << "' />" <<endl;
}

void
show_histogram_svg(const vector<size_t>& bins) {
    const auto IMAGE_WIDTH = 810;
    const auto IMAGE_HEIGHT = 300;
    const auto TEXT_LEFT = 20;
    const auto TEXT_BASELINE = 20;
    const auto TEXT_WIDTH = 50;
    const auto BIN_HEIGHT = 30;
    const auto BLOCK_WIDTH = 10;
    svg_begin(IMAGE_WIDTH, IMAGE_HEIGHT);
    const size_t SCREEN_WIDTH = 80;
    const size_t MAX_ASTERISK = SCREEN_WIDTH - 3 - 1;

    size_t max_count = 0;
    for (size_t count : bins) {
        if (count > max_count) {
            max_count = count;
        }
    }
    size_t kolichestvo_probelov=0;
    double top = 0;
for (size_t bin : bins) {
    const bool scaling_needed = max_count > MAX_ASTERISK;
    size_t height = bin;
        if (scaling_needed) {
            const double scaling_factor = (double)MAX_ASTERISK / max_count;
            height = (size_t)(bin * scaling_factor);
        }
    if (max_count < MAX_ASTERISK)
        kolichestvo_probelov = max_count - height;
    else
        kolichestvo_probelov = MAX_ASTERISK - height;
    const double bin_width = BLOCK_WIDTH * height;
    const double bin_probel = BLOCK_WIDTH * kolichestvo_probelov;
    svg_rect(0, top, bin_probel, BIN_HEIGHT, "white" , "#ffeceb");
    svg_rect(bin_probel, top, bin_width, BIN_HEIGHT, "white" , "#c71585");
    svg_text(TEXT_LEFT+bin_probel+bin_width, top + TEXT_BASELINE, to_string(bin));
    top += BIN_HEIGHT;
    }
    svg_end();
}
//

