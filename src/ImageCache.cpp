///\file OfflineReader.cpp
///\author Benjamin Knorlein
///\date 11/13/2016

#include "ImageCache.h"
#include <iostream>

ImageCache::ImageCache(ImageSource * source, int maxImages, int width, int height) : m_source(source), m_maxImages(maxImages), m_width(width), m_height(height)
{
	
}

ImageCache::~ImageCache()
{
	for (int i = 0; i < m_images.size(); i++){
		delete[] m_images_data[i];
		m_images[i].release();
	}

	m_images.clear();
	m_images_data.clear();
	m_depths.clear();
	m_mode.clear();
}

cv::Mat* ImageCache::getPhaseImage(int depth)
{
	int idx = findInCache(2, depth);
	if (idx < 0)
	{
		if (m_images.size() == m_maxImages) removeOldestImage();

		float* data = new float[m_width * m_height];
		m_source->getPhaseImage(depth, data);
		cv::Mat image(cv::Size(m_width, m_height), CV_32FC1, data);

		m_images.push_back(image);
		m_images_data.push_back(data);
		m_mode.push_back(2);
		m_depths.push_back(depth);
		idx = m_images.size() - 1;
		removeBorders(&(m_images[idx]), depth);
	}
	
	return &(m_images[idx]);
}

cv::Mat* ImageCache::getIntensityImage(int depth)
{
	int idx = findInCache(0, depth);
	if (idx < 0)
	{
		if (m_images.size() == m_maxImages) removeOldestImage();

		float* data = new float[m_width * m_height];
		m_source->getIntensityImage(depth, data);
		cv::Mat image(cv::Size(m_width, m_height), CV_32FC1, data);

		m_images.push_back(image);
		m_images_data.push_back(data);
		m_mode.push_back(0);
		m_depths.push_back(depth);
		idx = m_images.size() - 1;
		removeBorders(&(m_images[idx]), depth);
	}

	return &(m_images[idx]);
}

cv::Mat* ImageCache::getAmplitudeImage(int depth)
{
	int idx = findInCache(1, depth);
	if (idx < 0)
	{
		if (m_images.size() == m_maxImages) removeOldestImage();

		float* data = new float[m_width * m_height];
		m_source->getAmplitudeImage(depth, data);
		cv::Mat image(cv::Size(m_width, m_height), CV_32FC1, data);

		m_images.push_back(image);
		m_images_data.push_back(data);
		m_mode.push_back(1);
		m_depths.push_back(depth);
		idx = m_images.size() - 1;
		removeBorders(&(m_images[idx]), depth);
	}

	return &(m_images[idx]);
}


int ImageCache::findInCache(int mode, int depth)
{
	for (int i = 0; i < m_images.size(); i++)
	{
		if (m_mode[i] == mode && m_depths[i] == depth) return i;
	}
	return -1;
}

void ImageCache::removeOldestImage()
{
	delete[] m_images_data[0];
	m_images[0].release();

	m_images.erase(m_images.begin());
	m_images_data.erase(m_images_data.begin());
	m_depths.erase(m_depths.begin());
	m_mode.erase(m_mode.begin());
}

void ImageCache::removeBorders(cv::Mat* image, int depth)
{
	int size = 100;
	if (depth < 3000){
		size = 300;
	}
	else if (depth < 7000)
	{
		size = 150;
	}
	
	float val = 0;
	
	if (size > 0)
	{		
		(*image)(cv::Rect(0, 0, size, image->rows)) = val;
		(*image)(cv::Rect(0, 0, image->cols, size)) = val;
		(*image)(cv::Rect(image->cols - size, 0, size, image->rows)) = val;
		(*image)(cv::Rect(0, image->rows - size, image->cols, size)) = val;
	}
}
