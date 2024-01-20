#pragma once

#include <vector>
#include <iostream>
#include <chrono>
#include <algorithm>

class SessionData
{
public:
	float Time;
	int Stars;
};

class PlayerData
{
private:

	int maxSessions = 5;
	int musicVolume;
	std::vector<SessionData*> sessions;

public:

	PlayerData() : musicVolume(0), sessions() 
	{
		sessions = std::vector<SessionData*>();
	}

	PlayerData(int musicVolume) : musicVolume(musicVolume)
	{
		sessions = std::vector<SessionData*>();
	}

	~PlayerData()
	{
		for (SessionData* session : sessions)
		{
			delete session;
		}
	}

	void SetMusicVolume(int volume)
	{
		musicVolume = volume;
	}

	float GetMusicVolume()
	{
		return musicVolume;
	}

	std::vector<SessionData*> GetSessions()
	{
		return sessions;
	}

	void AddSession(float time, int stars)
	{
		if (HasSession(time, stars))
		{
			return;
		}

		if (sessions.size() >= maxSessions)
		{
			SessionData* lowestSession = GetLowestSession();

			if (stars < lowestSession->Stars)
			{
				return;
			}
			else if (stars == lowestSession->Stars && time > lowestSession->Time)
			{
				return;
			}

			lowestSession->Time = time;
			lowestSession->Stars = stars;

			return;
		}

		SessionData* session = new SessionData();
		session->Time = time;
		session->Stars = stars;
		sessions.push_back(session);
	}

	void SortSessions()
	{
		std::sort(sessions.begin(), sessions.end(), [](SessionData* a, SessionData* b) { return a->Stars > b->Stars; });
		std::sort(sessions.begin(), sessions.end(), [](SessionData* a, SessionData* b) { return a->Time < b->Time; });
	}

	SessionData* GetLowestSession()
	{
		SessionData* lowestSession = sessions[0];

		// check agaisnt time and stars

		for (SessionData* session : sessions)
		{
			if (session->Stars < lowestSession->Stars)
			{
				lowestSession = session;
			}
		}

		for (SessionData* session : sessions)
		{
			if (session->Stars == lowestSession->Stars && session->Time > lowestSession->Time)
			{
				lowestSession = session;
			}
		}
		return lowestSession;
	}

	bool HasSession(float time, int stars)
	{
		for (SessionData* session : sessions)
		{
			if (session->Stars == stars && session->Time == time)
			{
				return true;
			}
		}
		return false;
	}

	// Overload the stream operators for saving
	friend std::ostream& operator<<(std::ostream& out, const PlayerData& obj)
	{
		out << obj.musicVolume << " ";

		// Save the size of the vector
		out << obj.sessions.size() << " ";

		// Save the elements of the vector
		for (SessionData* session : obj.sessions)
		{
			out << session->Time << " " << session->Stars << " ";
		}
		return out;
	}

	// Overload the stream operators for loading
	friend std::istream& operator>>(std::istream& in, PlayerData& obj)
	{
		// Load money, gem, and characterID
		in >> obj.musicVolume;

		// Load the size of the inventory vector
		size_t sessionSize;
		in >> sessionSize;

		// Resize the inventory vector
		obj.sessions.resize(sessionSize);

		// Load each element of the inventory vector
		for (size_t i = 0; i < sessionSize; ++i) 
		{
			SessionData* session = new SessionData();
			in >> session->Time >> session->Stars;
			obj.sessions[i] = session;
		}
		return in;
	}
};
