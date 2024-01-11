#pragma once

#include <iostream>
#include <vector>
#include <functional>

template<class T>
class Pool
{
private:

	std::vector<T*> ActivePoolObject;
	std::vector<T*> UnactivePoolObjects;
	int DefaultSize;
	std::function <T* ()> OnCreate;
	std::function <void(T*)> OnGet;
	std::function <void(T*)> OnReturn;

public:

	Pool() {};

	Pool(std::function <T* ()> onCreate, std::function <void(T*)> onGet, std::function <void(T*)> onReturn, int defaultSize)
	{
		OnCreate = onCreate;
		OnGet = onGet;
		OnReturn = onReturn;

		ActivePoolObject = std::vector<T*>();
		UnactivePoolObjects = std::vector<T*>();

		for (size_t i = 0; i < defaultSize; i++)
		{
			T* newObject = onCreate();
			UnactivePoolObjects.push_back(newObject);
		}
	}

	~Pool()
	{
		for (size_t i = 0; i < ActivePoolObject.size(); i++)
		{
			delete ActivePoolObject[i];
			ActivePoolObject[i] = nullptr;
		}
		ActivePoolObject.clear();

		for (size_t i = 0; i < UnactivePoolObjects.size(); i++)
		{
			delete UnactivePoolObjects[i];
			UnactivePoolObjects[i] = nullptr;
		}
		UnactivePoolObjects.clear();
	}

	T* Get()
	{
		T* newObject;

		if (UnactivePoolObjects.size() == 0)
		{
			newObject = OnCreate();
		}
		else
		{
			newObject = UnactivePoolObjects.back();
			UnactivePoolObjects.pop_back();
		}

		OnGet(newObject);
		ActivePoolObject.push_back(newObject);
		return newObject;
	}

	void Return(T* toReturn)
	{
		OnReturn(toReturn);
		ActivePoolObject.erase(find(ActivePoolObject.begin(), ActivePoolObject.end(), toReturn));
		UnactivePoolObjects.push_back(toReturn);
	}

	bool Contains(std::vector<T*> vector, T* item)
	{
		for (size_t i = 0; i < vector.size(); i++)
		{
			if (vector[i] == item) return true;
		}
		return false;
	}

	std::vector<T*> GetActivePoolObjects()
	{
		return ActivePoolObject;
	}

	std::vector<T*> GetInActivePoolObjects()
	{
		return UnactivePoolObjects;
	}
};
