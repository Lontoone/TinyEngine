#pragma once

#include <fstream>
#include <istream>
#include <ostream>

namespace INANOA {
	namespace SCENE {
		namespace EXPERIMENTAL {
			class SpatialSample {
			public:
				explicit SpatialSample(const int numSample) :
					m_numSample(numSample), _DIM(3)
				{
					this->m_buffer = new float[numSample * _DIM * 2];
				}
				explicit SpatialSample(const int numSample, float* buffer) :
					m_numSample(numSample), m_buffer(buffer), _DIM(3) {}

				virtual ~SpatialSample() {
					delete[] this->m_buffer;
				}

				SpatialSample(const SpatialSample&) = delete;
				SpatialSample& operator=(const SpatialSample&) = delete;

			public:
				inline void setPosition(const int idx, const float x, const float y, const float z) {
					float* dst = this->m_buffer + idx * _DIM * 2;
					dst[0] = x; dst[1] = y; dst[2] = z;
				}
				inline void setRadian(const int idx, const float x, const float y, const float z) {
					float* dst = this->m_buffer + idx * _DIM * 2 + _DIM;
					dst[0] = x; dst[1] = y; dst[2] = z;
				}

			public:
				inline int numSample() const { return this->m_numSample; }
				inline const float* position(const int idx) const { return this->m_buffer + idx * _DIM * 2; }
				inline const float* radians(const int idx) const { return this->m_buffer + idx * _DIM * 2 + _DIM; }

			private:
				const int _DIM;
				int m_numSample = 0;
				float* m_buffer = nullptr;

			public:
				void exportBinaryFile(std::ostream& outputStream) {
					outputStream.write((char*)(&(this->m_numSample)), sizeof(int));
					outputStream.write((char*)(this->m_buffer), sizeof(float) * this->m_numSample * _DIM * 2);
				}

			public:
				static SpatialSample* importBinaryFile(std::istream& inputStream) {
					const int _DIM = 3;
					int numSample;
					inputStream.read((char*)(&numSample), sizeof(int));

					if (numSample <= 0) {
						return nullptr;
					}

					float* buffer = new float[numSample * _DIM * 2];

					inputStream.read((char*)(buffer), sizeof(float) * numSample * _DIM * 2);

					return new SpatialSample(numSample, buffer);
				}
				static SpatialSample* importBinaryFile(const std::string& filename) {
					std::ifstream input(filename, std::ios::binary);

					if (!input.is_open()) { return nullptr; }

					SpatialSample* samples = SpatialSample::importBinaryFile(input);
					input.close();
					return samples;
				}
			};
		}
	}
	
}
