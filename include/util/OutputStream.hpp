#pragma once
#include "fstream"
#include "iostream"
#include "Geode/Geode.hpp"
#include "Geode/loader/Log.hpp"
#include "Geode/binding/AdvancedFollowInstance.hpp"
#include "Geode/binding/CAState.hpp"
#include "Geode/binding/EnhancedGameObject.hpp"
#include "Geode/binding/EnterEffectInstance.hpp"
#include "Geode/binding/SongChannelState.hpp"
#include "Geode/binding/SongTriggerState.hpp"
#include "Geode/binding/SFXTriggerState.hpp"
#include "import_export.hpp"

#define PU_OPERATOR_WRITE(type) SABE_PU_DLL virtual void operator<<(type& i_value) { write(reinterpret_cast<char*>(&i_value), sizeof(type)); }

namespace persistencyUtils {
	class OutputStream {
	protected:
		std::ofstream* m_stream;
		const char m_zeros[256] = {};
	public:
		SABE_PU_DLL OutputStream() {
			m_stream = nullptr;
		};
		SABE_PU_DLL OutputStream(std::string i_filePath) { m_stream = new std::ofstream(i_filePath, std::ios_base::binary); }
		SABE_PU_DLL ~OutputStream() { delete m_stream; }
		
		PU_OPERATOR_WRITE(bool)
		PU_OPERATOR_WRITE(char)
		PU_OPERATOR_WRITE(unsigned char)
		PU_OPERATOR_WRITE(int)
		PU_OPERATOR_WRITE(unsigned int)
		PU_OPERATOR_WRITE(short)
		PU_OPERATOR_WRITE(unsigned short)
		PU_OPERATOR_WRITE(float)
		PU_OPERATOR_WRITE(double)
		PU_OPERATOR_WRITE(cocos2d::CCPoint)
		PU_OPERATOR_WRITE(cocos2d::CCSize)
		PU_OPERATOR_WRITE(cocos2d::CCAffineTransform)
		PU_OPERATOR_WRITE(uint64_t)

		SABE_PU_DLL bool setFileToWrite(std::string i_filePath) {
			if (m_stream) {
				delete m_stream;
			}
			m_stream = new std::ofstream(i_filePath, std::ios_base::binary);
			if (!m_stream->good()) {
				geode::log::info("Failed to open file path: {}", i_filePath);
				return false;
			}
			return true;
		}

		SABE_PU_DLL void write(char* i_value, int i_size) { m_stream->write(i_value, i_size); }
		SABE_PU_DLL void writeZero(int i_size) { 
			m_stream->write(m_zeros, i_size);
		}

		SABE_PU_DLL void seek(int i_pos) { m_stream->seekp(i_pos, std::ios_base::beg); }

		SABE_PU_DLL void end() {
			if (m_stream) {
				m_stream->flush();
				m_stream->close();
				delete m_stream;
			}
			m_stream = nullptr;
		}

		// custom operators

		// vector

		template <class T>
		SABE_PU_DLL void operator<<(gd::vector<T>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("VECTOR SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			write(reinterpret_cast<char*>(i_value.data()), l_size*sizeof(T));
		}

		template <>
		SABE_PU_DLL void operator<<<DynamicSaveObject>(gd::vector<DynamicSaveObject>& i_value);

		template <>
		SABE_PU_DLL void operator<<<ActiveSaveObject1>(gd::vector<ActiveSaveObject1>& i_value);

		template <>
		SABE_PU_DLL void operator<<<ActiveSaveObject2>(gd::vector<ActiveSaveObject2>& i_value);

		template <>
		SABE_PU_DLL void operator<<<CountTriggerAction>(gd::vector<CountTriggerAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<TouchToggleAction>(gd::vector<TouchToggleAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<CollisionTriggerAction>(gd::vector<CollisionTriggerAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<ToggleTriggerAction>(gd::vector<ToggleTriggerAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<SpawnTriggerAction>(gd::vector<SpawnTriggerAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<GroupCommandObject2>(gd::vector<GroupCommandObject2>& i_value);

		template <>
		SABE_PU_DLL void operator<<<KeyframeObject>(gd::vector<KeyframeObject>& i_value);

		template <>
		SABE_PU_DLL void operator<<<TimerTriggerAction>(gd::vector<TimerTriggerAction>& i_value);

		template <>
		SABE_PU_DLL void operator<<<EventTriggerInstance>(gd::vector<EventTriggerInstance>& i_value);

		template <>
		SABE_PU_DLL void operator<<<EnterEffectInstance>(gd::vector<EnterEffectInstance>& i_value);

		template <>
		SABE_PU_DLL void operator<<<AdvancedFollowInstance>(gd::vector<AdvancedFollowInstance>& i_value);

		template <>
		SABE_PU_DLL void operator<<<CAState>(gd::vector<CAState>& i_value);

		template <>
		SABE_PU_DLL void operator<<<SongTriggerState>(gd::vector<SongTriggerState>& i_value);

		template <>
		SABE_PU_DLL void operator<<<SFXTriggerState>(gd::vector<SFXTriggerState>& i_value);

		template <>
		SABE_PU_DLL void operator<<<DynamicObjectAction>(gd::vector<DynamicObjectAction>& i_value);

		// unordered_map

		template <class K, class V>
		SABE_PU_DLL void operator<<(gd::unordered_map<K,V>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Unordered Map SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (std::pair<K,V> l_pair : i_value) {
				write(reinterpret_cast<char*>(&l_pair.first), sizeof(K));
				write(reinterpret_cast<char*>(&l_pair.second), sizeof(V));
			}
		}

		template <class K, class V>
		SABE_PU_DLL void operator<<(gd::unordered_map<K,gd::vector<V>>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Unordered Map key->vector<T> SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (std::pair<K,gd::vector<V>> l_pair : i_value) {
				write(reinterpret_cast<char*>(&l_pair.first), sizeof(K));
				*this << l_pair.second; 
			}
		}

		template <>
		SABE_PU_DLL void operator<<<int, SequenceTriggerState>(gd::unordered_map<int, SequenceTriggerState>& i_value);

		template <>
		SABE_PU_DLL void operator<<<int, FMODQueuedMusic>(gd::unordered_map<int, FMODQueuedMusic>& i_value);

		template <>
		SABE_PU_DLL void operator<<<int, FMODSoundState>(gd::unordered_map<int, FMODSoundState>& i_value);

		template <>
		SABE_PU_DLL void operator<<<int, TimerItem>(gd::unordered_map<int, TimerItem>& i_value);

		template <>
		SABE_PU_DLL void operator<<<int, EnhancedGameObject*>(gd::unordered_map<int, EnhancedGameObject*>& i_value);

		template <>
		SABE_PU_DLL void operator<<<int, SongChannelState>(gd::unordered_map<int, SongChannelState>& i_value);

		// unordered_set

		template <class K>
		SABE_PU_DLL void operator<<(gd::unordered_set<K>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Unordered Set SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (K l_key : i_value) {
				write(reinterpret_cast<char*>(&l_key), sizeof(K));
			}
		}

		// map

		template <class K, class V>
		SABE_PU_DLL void operator<<(gd::map<K,V>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Map SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (std::pair<K,V> l_pair : i_value) {
				write(reinterpret_cast<char*>(&l_pair.first), sizeof(K));
				write(reinterpret_cast<char*>(&l_pair.second), sizeof(V));
			}
		}

		template <class K, class V>
		SABE_PU_DLL void operator<<(gd::map<K,gd::vector<V>>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Unordered Map key->vector<T> SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (std::pair<K,gd::vector<V>> l_pair : i_value) {
				write(reinterpret_cast<char*>(&l_pair.first), sizeof(K));
				*this << l_pair.second; 
			}
		}

		template <>
		SABE_PU_DLL void operator<<<std::pair<int,int>, SFXTriggerInstance>(gd::map<std::pair<int,int>, SFXTriggerInstance>& i_value);

		// set

		template <class K>
		SABE_PU_DLL void operator<<(gd::set<K>& i_value) {
			unsigned int l_size = i_value.size();
			//geode::log::info("Set SIZE out: {}", l_size);
			write(reinterpret_cast<char*>(&l_size), 4);
			for (K l_key : i_value) {
				write(reinterpret_cast<char*>(&l_key), sizeof(K));
			}
		}

		// gd::string

		SABE_PU_DLL void operator<<(gd::string& i_value) {
			unsigned int l_size = i_value.size();
			write(reinterpret_cast<char*>(&l_size), 4);
			//geode::log::info("String SIZE in: {}", l_size);
			if (l_size == 0) return;
			write(i_value.data(), l_size);
		}
	};
}