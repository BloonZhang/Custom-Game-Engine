#pragma once

#include <cassert>
#include <chrono>
#include <functional>

namespace Fiea::GameEngine
{
	/** USAGE:
	 *   game() {
	 *       GameClock clock;
	 *       GameTime time = clock.Current();
	 *
	 *       // game loop
	 *       while(1) {
	 *           while(clock.Elapsed(time) < FRAME_TICKS) sleep();
	 * 
	 *           // update the time as we enter a new frame
	 *           clock.Update(time);
	 *
	 *           // update game using the time
	 *           root.Update(time);
	 *       }
	 *  }
	 * 
	 *  Root::Update(const GameTime& time) {
	 *      // for handling typical frame-by-frame updates
	 *      updateSinceLast(time.Frame());
	 * 
	 *      // for handling things scheduled in real time
	 *      processAbsolute(time.Get());
	 * 
	 *      // for handling things scheduled "since boot"
	 *      processSinceStart(time.Game());
	 *  }
 	 */

	using Millis = long long;

	class GameClock;

	/**
	 * @brief A simple struct which can provide indirect access to a snapshot of the clock
	 *         information, as well as elapsed frame time
	*/
	struct GameTime final
	{
		friend GameClock;
		friend static inline GameTime CreateElapsedTime(int ms);
	public:
		/**
		 * @brief Milliseconds since epoch
		 * @return milliseconds since epoch
		*/
		inline Millis Get() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current.time_since_epoch()).count(); };
		/**
		 * @brief Milliseconds since start
		 * @return milliseconds since start
		*/
		inline Millis Game() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current - _start).count(); };
		/**
		 * @brief Milliseconds since last frame
		 * @return milliseconds since last frame
		*/
		inline Millis Frame() const { return std::chrono::duration_cast<std::chrono::milliseconds>(_current - _last).count(); };

	private:
		GameTime() {}

		std::chrono::high_resolution_clock::time_point _start{};
		std::chrono::high_resolution_clock::time_point _last{};
		std::chrono::high_resolution_clock::time_point _current{};
	};

	/**
	 * @brief A class to wrap the system clock and provide additional functionality
	*/
	class GameClock final
	{
	public:
		/**
		 * @brief Any function which returns a time_point. Provided for use in the
		 *        debug default constructor, so that "fake" time may be maintained by 
		 *        the application, for use in debug code
		*/
		//using now_func = std::function<std::chrono::high_resolution_clock::time_point()>;

		// Make it a singleton
		inline static GameClock* Instance;

		/// @brief initialize_singleton
		/// Initializes the singleton by created a new instance
		/// Throws an exception if it has already been initialized
		static void initialize_singleton()
		{
			// if already initialized, then throw an exception
			if (Instance != nullptr) { throw std::invalid_argument("Tried to initialize GameClock when it has already been intialized."); }
			// Otherwise, create a new instance and set it as the singleton instance
			Instance = new GameClock();
			// Also set the start time
			Instance->_startTime = std::chrono::high_resolution_clock::now();
		}

		/// @brief destroy_singleton
		/// Frees the memory of the singleton instance
		static void destroy_singleton()
		{
			delete Instance;
			Instance = nullptr;
		}

		/*
		GameClock(now_func now = nullptr);
		GameClock(const GameClock&) = default;
		GameClock& operator=(const GameClock&) = default;
		GameClock(GameClock&&) = default;
		GameClock& operator=(GameClock&&) = default;
		~GameClock() = default;
		*/

		/**
		 * @brief Return a GameTime struct based on the current clock time
		 * @return a game time struct
		*/
		//[[nodiscard]] GameTime Current() const;
		[[nodiscard]] static GameTime Current()
		{
			GameTime time;
			time._start = time._start = Instance->_startTime;
			time._current = std::chrono::high_resolution_clock::now();
			return time;
		}
		/**
		 * @brief Given a game time struct, how much time has elapsed since it was last updated
		*/
		//Millis Elapsed(const GameTime& time) const;
		static Millis Elapsed(const GameTime& time)
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time._last).count();
		}

		/**
		 * @brief Updates a game time struct, based upon the current clock time
		*/
		//void Update(GameTime& time) const;
		static void Update(GameTime& time)
		{
			time._last = time._current;
			time._current = std::chrono::high_resolution_clock::now();
		}
		/**
		* @brief For testing purposes, forces GameClock to have a certain Frame() value
		*/
		//void UpdateForced(GameTime& time, long long forcedTime) const;

	private:
		//now_func _now = nullptr;

		std::chrono::high_resolution_clock::time_point _startTime;
	};

	// Debug free function for creating a fake elapsed time
	static inline GameTime CreateElapsedTime(int ms) 
	{
		/*
		std::chrono::time_point now = std::chrono::high_resolution_clock::now();
		GameClock clock([&now]() { return now; });
		GameTime time = clock.Current();
		now += std::chrono::milliseconds(ms);
		clock.Update(time);
		return time;
		*/
		//std::chrono::time_point now = std::chrono::high_resolution_clock::now();
		//GameClock clock([&now]() { return now; });
		GameTime time = GameClock::Current();
		std::chrono::high_resolution_clock::duration tempDuration(ms * 1000000);
		time._last = time._current;
		time._current = time._last + tempDuration;
		return time;
	}
}