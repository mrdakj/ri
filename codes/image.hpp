#ifndef IMAGE_HPP
#define IMAGE_HPP 

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <experimental/filesystem>
#include <numeric>
#include <string>
#include <thread>
#include <future>


namespace img {
	enum class Type { GRAYSCALE, RGB };
	enum Color { BLACK = 0, WHITE = 255 };
	enum { R = 2, G = 1, B = 0 };

	template <Type t = Type::RGB>
	class Image {
	private:
		std::string m_name;
        cv::Mat m_data;

	public:
        Image(const std::experimental::filesystem::path& path);
        Image(unsigned rows = 0, unsigned cols = 0, Color c = Color::WHITE, std::string name = "");
		Image(const cv::Mat& data)
			: m_name(), m_data(data)
        {}

        Image(cv::Mat&& data)
            : m_name(), m_data(std::move(data))
        {}

        Image(const Image& other)
            : m_name(other.m_name), m_data(other.m_data.clone())
        {}

        // we need this because we have defined copy constructor
        Image(Image&& other) = default;
        Image& operator=(const Image& other) = default;
        Image& operator=(Image&& other) = default;

		int rows() const
		{
			return m_data.rows;
		}

		int cols() const
		{
			return m_data.cols;
		}


		void save(const std::experimental::filesystem::path& path) const
		{
            if (!path.empty() && !m_data.empty())
                imwrite(path.string(), m_data);
		}

		unsigned red(unsigned i, unsigned j) const;
		unsigned green(unsigned i, unsigned j) const;
		unsigned blue(unsigned i, unsigned j) const;

		Image<Type::RGB> fuzzy_color_contrast() const;
		Image<Type::GRAYSCALE> fuzzy_grayscale_contrast() const;
		
		std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j)
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(i,j);
		}

		const std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(unsigned i, unsigned j) const
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(i,j);
		}

		std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(const std::pair<unsigned, unsigned>& p)
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(p.first, p.second);
		}

		const std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>& operator()(const std::pair<unsigned, unsigned>& p) const 
		{
			return m_data.at<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>(p.first, p.second);
		}


		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> begin()
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> begin() const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cbegin() const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}


		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> end()
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> end() const
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> cend() const
		{
			return m_data.end<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>();
		}

		std::conditional_t<t==Type::RGB, cv::MatIterator_<cv::Vec3b>, cv::MatIterator_<unsigned char>> operator[](unsigned i)
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>() + i*cols();
		}

		std::conditional_t<t==Type::RGB, cv::MatConstIterator_<cv::Vec3b>, cv::MatConstIterator_<unsigned char>> operator[](unsigned i) const
		{
			return m_data.begin<std::conditional_t<t==Type::RGB, cv::Vec3b, unsigned char>>() + i*cols();
		}

	};

}; // end of namespace img
#endif /* IMAGE_HPP */
