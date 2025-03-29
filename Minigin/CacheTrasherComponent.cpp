#include "CacheTrasherComponent.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <numeric>
#include "Renderer.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"


void CacheTrasherComponent::IntegerHeapTest(int samples)
{

	if (samples <= 0) return;

	m_FunctionTimingIntegers.clear();

	const int size{ (int)std::powf(2,26) };
	int* heap = new int[size] {};


	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<int> time;
		for (int sample{}; sample < samples; ++sample)
		{
			const auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < size; i += stepsize)
			{
				heap[i] *= 2;
			}
			const auto stopTime = std::chrono::high_resolution_clock::now();
			time.emplace_back((int) std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count());
		}

		long long average = (std::accumulate(time.begin(), time.end(), 0)) / time.size();

		std::pair<int, float> pair{ stepsize, (float) average };
		m_FunctionTimingIntegers.insert(pair);
	
	}
	delete[] heap;
}

void CacheTrasherComponent::ObjectHeapTest(int samples)
{
	if (samples <= 0) return;

	m_FunctionTimingObjects.clear();

	const int size{ (int)std::powf(2,26) };
	HeapTestObject* heap = new HeapTestObject[size]{};


	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<int> time;
		for (int sample{}; sample < samples; ++sample)
		{
			const auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < size; i += stepsize)
			{
				heap[i].id *= 2;
			}
			const auto stopTime = std::chrono::high_resolution_clock::now();
			time.emplace_back((int) std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count());
		}

		long long average = (std::accumulate(time.begin(), time.end(), 0)) / time.size();
		std::pair<int, float> pair{ stepsize,(float)average };
		m_FunctionTimingObjects.insert(pair);

	}
	delete[] heap;
}

void CacheTrasherComponent::ObjectWithPointerHeapTest(int  samples)
{

	if (samples <= 0) return;

	m_FunctionTimingObjectsWithPtrs.clear();

	const int size{ (int)std::powf(2,26) };
	HeapTestObjectWithPtr* heap = new HeapTestObjectWithPtr[size]{};


	for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
	{
		std::vector<int> time;
		for (int sample{}; sample < samples; ++sample)
		{
			const auto startTime = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < size; i += stepsize)
			{
				heap[i].id *= 2;
			}
			const auto stopTime = std::chrono::high_resolution_clock::now();
			time.emplace_back((int) std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count());
		}

		long long average = (std::accumulate(time.begin(), time.end(), 0)) / time.size();
		std::pair<int, float> pair{ stepsize, (float) average };
		m_FunctionTimingObjectsWithPtrs.insert(pair);
	
	}
	delete[] heap;

}

void CacheTrasherComponent::Render()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();



	if (ImGui::Begin("Exercise 2"))
	{

		ImGui::InputInt("Samples", &m_SamplesIntegers);
		if (!m_FunctionTimingIntegers.empty())
		{
			if (ImPlot::BeginPlot("My Plot"))
			{
				std::vector<double> x, y;
				for (const std::pair<int, double>& pair : m_FunctionTimingIntegers)
				{
					x.emplace_back((pair.first));
					y.emplace_back((pair.second));
				}
				ImPlot::PlotLine("Time", &x[0], &y[0], (int)x.size());
				ImPlot::EndPlot();
			}
		}
		if (ImGui::Button("Trash the cache"))
		{
			IntegerHeapTest(m_SamplesIntegers);
		}
	}
	ImGui::End();
	if (ImGui::Begin("Exercise 3"))
	{

		ImGui::InputInt("Samples", &m_SamplesObjects);
		if (!m_FunctionTimingObjects.empty())
		{
			if (ImPlot::BeginPlot("GameObject3D"))
			{
				std::vector<double> x, y;
				for (const std::pair<int, double>& pair : m_FunctionTimingObjects)
				{
					x.emplace_back((pair.first));
					y.emplace_back((pair.second));
				}
				ImPlot::PlotLine("Time", &x[0], &y[0], (int)x.size());
				ImPlot::EndPlot();
			}
		}
		if (ImGui::Button("Trash the cache with GameObject3D"))
		{
			ObjectHeapTest(m_SamplesObjects);
		}
		if (!m_FunctionTimingObjectsWithPtrs.empty())
		{
			if (ImPlot::BeginPlot("GameObjects3DAlt"))
			{
				std::vector<double> x, y;
				for (const std::pair<int, double>& pair : m_FunctionTimingObjectsWithPtrs)
				{
					x.emplace_back((pair.first));
					y.emplace_back((pair.second));
				}
				ImPlot::PlotLine("Time", &x[0], &y[0], (int)x.size());
				ImPlot::EndPlot();
			}
		}

		if (ImGui::Button("Trash the cache with GameObject3DAlt"))
		{
			ObjectWithPointerHeapTest(m_SamplesObjects);


		}
		ImGui::End();

		if (!m_FunctionTimingObjects.empty() && !m_FunctionTimingObjectsWithPtrs.empty())
		{
			if (ImPlot::BeginPlot("GameObject3D vs GameObject3DAlt"))
			{
				std::vector<double> x, y, y2;
				for (const std::pair<int, double>& pair : m_FunctionTimingObjects)
				{
					x.emplace_back((pair.first));
					y.emplace_back((pair.second));
				}
				for (const std::pair<int, double>& pair : m_FunctionTimingObjectsWithPtrs)
				{
					y2.emplace_back((pair.second));
				}
				ImPlot::PlotLine("GameObject3D", &x[0], &y[0], (int)x.size());
				ImPlot::PlotLine("GameObject3DAlt", &x[0], &y2[0], (int)x.size());
				ImPlot::EndPlot();
			}
		}



		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}

	
