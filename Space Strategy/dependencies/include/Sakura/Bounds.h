#pragma once
#include <glm/glm.hpp>

namespace Sakura{

	struct Circle{
	public:
		/** Negetive Radius produces unexpected results
		* Use positive redius values only */
		Circle(float X, float Y, float Radius){
			x = X;
			y = Y;
			radius = Radius;
		}

		Circle& operator=(Circle& ref){
			this->x = ref.x;
			this->y = ref.y;
			this->radius = ref.radius;
			return *this;
		}

		float x;
		float y;
		float radius;
	};

	struct Rect{
	public:
		Rect(){ /* Empty */ }
		/** Negetive width and height values produce unexpected results
		* Use positive width and height values only */
		Rect(float X, float Y, float W, float H, bool stationary){
			width = W;
			height = H;
			velocity = glm::vec2(0.0f);
			x1 = X;
			x2 = x1 + width;
			y2 = Y;
			y1 = y2 + height;
			isStationary = stationary;
		}

		void initialize(float X, float Y, float W, float H, bool stationary){
			width = W;
			height = H;
			velocity = glm::vec2(0.0f);
			x1 = X;
			x2 = x1 + width;
			y2 = Y;
			y1 = y2 + height;
			isStationary = stationary;
		}

		void move(float xAdd, float yAdd){
			x1 += xAdd;
			x2 += xAdd;
			y1 += yAdd;
			y2 += yAdd;
		}

		void update(float deltaTime){
			x1 += (velocity.x * deltaTime);
			x2 += (velocity.x * deltaTime);
			y1 += (velocity.y * deltaTime);
			y2 += (velocity.y * deltaTime);
		}

		/* Assignment Operator */
		Rect &operator=(Rect& ref){
			this->x1 = ref.x1;
			this->x2 = ref.x2;
			this->y1 = ref.y1;
			this->y2 = ref.y2;
			this->velocity = ref.velocity;
			this->height = ref.height;
			this->width = ref.width;

			return *this;
		}

		/* Rectangle Intersection Operator */
		bool intersect(Rect& ref) const {
			return (this->x1 <= ref.x2 && this->x2 >= ref.x1 &&
					this->y1 >= ref.y2 && this->y2 <= ref.y1);
		}

		/* Check if a point is in rect */
		bool pointIntersection(float x, float y) const{
			return (x >= this->x1 && x <= this->x2 &&
				y >= this->y2 && y <= this->y1);
		}

		bool calculateRectangleCollision(Rect& ref);

		/* Circle Intersection Operator */
		const bool operator()(const Circle& ref){
			// TODO : CIRCLE INTERSECTION DETECTION
			return false;
		}

		/* X1 and Y1 are values of the top left corners */
		float x1;
		float x2;
		float y1;
		float y2;

		bool isStationary = true;

		glm::vec2 velocity;

		float width;
		float height;
	};
}

