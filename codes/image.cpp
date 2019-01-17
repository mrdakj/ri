#include "image.hpp"
using namespace img;

#define MAX_COLOR_LEVEL 255.0
#define NUM_CHANNELS    3

template <>
Image<Type::RGB>::Image(const std::experimental::filesystem::path& path)
    : m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_COLOR))
{
}

template <>
Image<Type::GRAYSCALE>::Image(const std::experimental::filesystem::path& path)
    : m_name(path.filename()), m_data(cv::imread(path.string(), cv::IMREAD_GRAYSCALE))
{
}

template <>
Image<Type::RGB>::Image(unsigned rows, unsigned cols, Color c, std::string name)
    : m_name(std::move(name)), m_data(cv::Mat(rows, cols, CV_8UC3, cv::Scalar(c, c, c)))
{
}

template <>
Image<Type::GRAYSCALE>::Image(unsigned rows, unsigned cols, Color c, std::string name)
    : m_name(std::move(name)), m_data(cv::Mat(rows, cols, CV_8UC1, cv::Scalar(c)))
{
}

template<>
unsigned Image<Type::RGB>::red(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[R];
}

template<>
unsigned Image<Type::RGB>::green(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[G];
}

template<>
unsigned Image<Type::RGB>::blue(unsigned i, unsigned j) const
{
	return m_data.at<cv::Vec3b>(i, j)[B];
}

template<>
Image<Type::RGB> Image<Type::RGB>::fuzzy_color_contrast() const
{
    // "A Highly Efficient Color Image Contrast Enhancement using Fuzzy Based Contrast Intensification Operator"
    // Pooja Mishra, Khom Lal Sinha
    // Advanced Research in Electrical and Electronic Engineering; Volume 1, Number 4 (2014)

    img::Image<img::Type::RGB> output(rows(), cols());

    unsigned max_R, min_R;
    unsigned max_G, min_G;
    unsigned max_B, min_B;
    unsigned R, G, B;
    
    max_R = min_R = red(0, 0);
    max_G = min_G = green(0, 0);
    max_B = min_B = blue(0, 0);
        
    for (int i = 1; i < rows(); i++) {
        for (int j = 1; j <  cols(); ++j) {
            R = red(i, j);
            G = green(i, j);
            B = blue(i, j);

            if (max_R < R) max_R = R;
            else if (min_R > R) min_R = R;
        
            if (max_G < G) max_G = G;
            else if (min_G > G) min_G = G;

            if (max_B < B) max_B = B;
            else if (min_B > B) min_B = B;
            }
    }
 
    std::vector<unsigned> max_RGB {max_R, max_G, max_B};
    std::vector<double> mid_RGB {(max_R - min_R) / 2.0, (max_G - min_G) / 2.0, (max_B - min_B) / 2.0};
    
    std::vector<unsigned> RGB;
    RGB.reserve(3);
    std::vector<unsigned> curr_RGB;
    curr_RGB.reserve(3);

    double Fe = 2.0;
    double Fd;

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j < cols(); ++j) {
            curr_RGB.resize(0);
            curr_RGB.push_back(red(i, j)); 
            curr_RGB.push_back(green(i, j));
            curr_RGB.push_back(blue(i, j));
            RGB.resize(0);

            for (int c = 0; c < NUM_CHANNELS; ++c) {
                Fd = (max_RGB[c] - mid_RGB[c]) / 0.41421356237309515;
                // fuzzification
                double mem_func = pow(1 + (max_RGB[c] - curr_RGB[c]) / Fd, -Fe);
                
                // modification of membership function by INT operator
                if (mem_func <= 0.5 and mem_func > 0) {
                    mem_func = 2 * pow(mem_func, 2);
                }
                else if (mem_func <= 1 and mem_func > 0.5) {
                    mem_func = 1 - 2 * pow(1 - mem_func, 2);
                }

                // defuzzification
                RGB.push_back(max_RGB[c] - Fd * (pow(mem_func, -1 / Fe)) + Fd);
            }
            
            cv::Vec3f BGR(RGB[2], RGB[1], RGB[0]);
            output(i, j) = BGR;
        }
    }

    return output;
}

template<>
Image<Type::GRAYSCALE> Image<Type::GRAYSCALE>::fuzzy_grayscale_contrast() const
{
    // "Fuzzy based Low Contrast Image Enhancement Technique by using Pal and King Method"
    // Ajay Kumar Gupta, Siddharth Singh Chauhan, Manish Shrivastava
    // International Journal of Computer Applications Volume 141 – No.6, May 2016
    
    img::Image<img::Type::GRAYSCALE> output(rows(), cols());

    unsigned min_gray = *std::min_element((*this).cbegin(), (*this).cend());
    unsigned max_gray = *std::max_element((*this).cbegin(), (*this).cend());
    unsigned diff = max_gray - min_gray;

    for (int i = 0; i < rows(); i++) {
        for (int j = 0; j <  cols(); ++j) {
            // fuzzification
            double mem_func = ((*this)(i, j) - min_gray) / double(diff);
            
            // modification of membership function by INT operator
            if (mem_func <= 0.5 and mem_func > 0) {
                mem_func = 2 * pow(mem_func, 2);
            }
            else if (mem_func <= 1 and mem_func > 0.5) {
                mem_func = 1 - 2 * pow(1 - mem_func, 2);
            }

            // defuzzification
            output(i, j) = min_gray + mem_func * diff; 
        }
    }
    
    return output;
}
