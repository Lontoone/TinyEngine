#pragma once

#include <glm\vec3.hpp>
#include <glm\gtx\rotate_vector.hpp>
#include <random>

namespace INANOA {
	namespace SCENE {
		namespace EXPERIMENTAL {
			class Trajectory {
			public:
				explicit Trajectory() {
					this->m_direction = glm::vec3(1.0, 0.0, 0.0);
					this->setStartPosition(glm::vec3(0.0, 0.0, -50.0));

					std::random_device seedGen;
					this->m_engine = std::mt19937(seedGen());
					this->m_dist = std::normal_distribution<double>(0.0, 0.03);
				}

				void setStartPosition(const glm::vec3& startPos) {
					this->m_currPosition = startPos;
				}
				void update() {
					if (this->m_enabled == false) {
						return;
					}
					if (this->m_frameCounter > 30) {
						float radiansDelta = this->m_dist(this->m_engine);
						this->m_radians = this->m_radians + radiansDelta;

						this->m_radians = glm::clamp(this->m_radians, -0.1f, 0.1f);

						this->m_direction = glm::rotateY(this->m_direction, this->m_radians);
						this->m_frameCounter = 0;
					}
					else {
						this->m_frameCounter = this->m_frameCounter + 1;
					}

					this->m_currPosition = this->m_currPosition + this->m_speed * this->m_direction;
					// boundary
					if (this->m_currPosition.x > 50.0 || this->m_currPosition.x < -50.0 || this->m_currPosition.z > 10.0 || this->m_currPosition.z < -250.0) {
						this->m_direction = -1.0f * this->m_direction;
					}
				}

			public:
				inline void enable(const bool flag) { this->m_enabled = flag; }

			public:
				inline glm::vec3 position() const {
					return this->m_currPosition;
				}
				inline glm::vec4 positionVec4() const {
					return glm::vec4(this->m_currPosition, 1.0);
				}

			private:
				glm::vec3 m_currPosition;
				glm::vec3 m_direction;
				float m_speed = 0.01;
				float m_radians = 0.0;

				std::normal_distribution<double> m_dist;
				std::mt19937 m_engine;

				int m_frameCounter = 0;

				bool m_enabled = true;
			};
		}		
	}
}
