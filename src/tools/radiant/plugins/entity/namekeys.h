/*
 Copyright (C) 2001-2006, William Joseph.
 All Rights Reserved.

 This file is part of GtkRadiant.

 GtkRadiant is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 GtkRadiant is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with GtkRadiant; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#if !defined(INCLUDED_NAMEKEYS_H)
#define INCLUDED_NAMEKEYS_H

#include <stdio.h>
#include <map>
#include "generic/static.h"
#include "entitylib.h"
#include "namespace.h"

inline bool string_is_integer (const char* string)
{
	if (strtol(string, const_cast<char**> (&string), 10) == LONG_MAX)
		return false;
	return *string == '\0';
}

static inline bool keyIsName (const char* key)
{
	return string_equal(key, "target") || string_equal(key, "targetname");
}

typedef MemberCaller1<EntityKeyValue, const char*, &EntityKeyValue::assign> KeyValueAssignCaller;
typedef MemberCaller1<EntityKeyValue, const KeyObserver&, &EntityKeyValue::attach> KeyValueAttachCaller;
typedef MemberCaller1<EntityKeyValue, const KeyObserver&, &EntityKeyValue::detach> KeyValueDetachCaller;

class NameKeys: public Entity::Observer, public Namespaced
{
		Namespace* m_namespace;
		EntityKeyValues& m_entity;
		NameKeys (const NameKeys& other);
		NameKeys& operator= (const NameKeys& other);

		typedef std::map<CopiedString, EntityKeyValue*> KeyValues;
		KeyValues m_keyValues;

		void insertName (const char* key, EntityKeyValue& value)
		{
			if (m_namespace != 0 && keyIsName(key)) {
				//globalOutputStream() << "insert " << key << "\n";
				m_namespace->attach(KeyValueAssignCaller(value), KeyValueAttachCaller(value));
			}
		}
		void eraseName (const char* key, EntityKeyValue& value)
		{
			if (m_namespace != 0 && keyIsName(key)) {
				//globalOutputStream() << "erase " << key << "\n";
				m_namespace->detach(KeyValueAssignCaller(value), KeyValueDetachCaller(value));
			}
		}
		void insertAll ()
		{
			for (KeyValues::iterator i = m_keyValues.begin(); i != m_keyValues.end(); ++i) {
				insertName((*i).first.c_str(), *(*i).second);
			}
		}
		void eraseAll ()
		{
			for (KeyValues::iterator i = m_keyValues.begin(); i != m_keyValues.end(); ++i) {
				eraseName((*i).first.c_str(), *(*i).second);
			}
		}
	public:
		NameKeys (EntityKeyValues& entity) :
			m_namespace(0), m_entity(entity)
		{
			m_entity.attach(*this);
		}
		~NameKeys ()
		{
			m_entity.detach(*this);
		}
		void setNamespace (Namespace& space)
		{
			eraseAll();
			m_namespace = &space;
			insertAll();
		}
		void insert (const char* key, EntityKeyValue& value)
		{
			m_keyValues.insert(KeyValues::value_type(key, &value));
			insertName(key, value);
		}
		void erase (const char* key, EntityKeyValue& value)
		{
			eraseName(key, value);
			m_keyValues.erase(key);
		}
};

#endif
