#ifndef FOOTBALL_REFEREE_H_
#define FOOTBALL_REFEREE_H_	

#include "football/Behavior.h"

namespace Football {

	class RefereeControllerFactory {
		public:
			RefereeControllerFactory(GameObjectCollection &, float);
			virtual std::unique_ptr<ObjectController> create(ControllableObject &) = 0;
		protected:
			GameObjectCollection &gameObjects;
			float speed;
	};
	
	class RefereeController : public BehaviorController {
		public:
			RefereeController(ControllableObject &, GameObjectCollection &, float);
			virtual void reset(PassiveObject &) = 0;
	};
	
	class Referee : public GameObject {
		public:
			Referee(Drawable &, RefereeControllerFactory &);
			RefereeController &getBehavior();
			void reset(PassiveObject &);
	};

	class LinearRefereeController : public RefereeController {
		public:
			LinearRefereeController(ControllableObject &, GameObjectCollection &, float, float);
			void reset(PassiveObject &) override;
		protected:
			virtual void play(PassiveObject &, float);
		private:
			float line;
	};
	
	class LinearRefereeControllerFactory : public RefereeControllerFactory {
		public:
			LinearRefereeControllerFactory(GameObjectCollection &, float, float);
			std::unique_ptr<ObjectController> create(ControllableObject &) override;
			void setLine(float);
		private:
			float line;
	};

	class MainRefereeController : public RefereeController {
		public:
			MainRefereeController(ControllableObject &, GameObjectCollection &, float, float, float);
			void reset(PassiveObject &) override;
		protected:
			void checkChaseOffset();
			void newChaseOffset();
			virtual void play(PassiveObject &, float);
			Position chaseOffset;
			float offsetChangeInterval;
			float initOffsetChangeInterval;
			float radius;
	};
	
	class MainRefereeControllerFactory : public RefereeControllerFactory {
		public:
			MainRefereeControllerFactory(GameObjectCollection &, float, float, float);
			std::unique_ptr<ObjectController> create(ControllableObject &) override;
		private:
			float initOffsetChangeInterval;
			float radius;
	};
}

#endif