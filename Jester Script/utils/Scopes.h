#ifndef SCOPES_H
#define SCOPES_H

#include "../src/Types.h"
#include "../src/VM.h"

using namespace jts;

class Scope
{
	using map = std::unordered_map<str, Obj*>;

public:

	Scope* emplace_scope()
	{
		Scope* scope = new Scope();
		scope->m_parent = this;

		return m_childScopes.emplace_back(scope);
	}

	Scope* get_scope(size_t ind)
	{
		return m_childScopes[ind];
	}

	void new_object(str& symbol, Obj* obj)
	{
		m_map.emplace(symbol, obj);
	}

	Obj* get_object(VM* vm, str& key)
	{
		Obj* obj = env::getSymbol(vm, key);
		if (obj) return obj;

		if (m_map.find(key) != m_map.end()) return m_map[key];

		for (Scope* scope : m_childScopes)
		{
			Obj* value = scope->get_object_impl(key);
			if (value) return value;
		}

		return nullptr;
	}

	Scope* get_parent()
	{
		return m_parent;
	}

	void move_to(map& targetMap, bool recursive = true)
	{
		for (auto& elem : m_map)
		{
			targetMap.emplace(elem.first, elem.second);
		}

		if (!recursive) return;

		for (Scope* scope : m_childScopes)
		{
			scope->move_to(targetMap);
		}
	}

private:

	Obj* get_object_impl(str& key)
	{
		if (m_map.find(key) != m_map.end()) return m_map[key];

		for (Scope* scope : m_childScopes)
		{
			Obj* value = scope->get_object_impl(key);
			if (value) return value;
		}

		return nullptr;
	}

	map m_map;
	std::vector<Scope*> m_childScopes;
	Scope* m_parent;
};

#endif