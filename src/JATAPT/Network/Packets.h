#pragma once

#include <JATAPT/JATAPT_COMMON.h>

#include <cereal/archives/binary.hpp>
#include <sstream>
#include <cereal/types/string.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/chrono.hpp>
#include <fstream>


namespace JATAPT
{
	namespace COMMON
	{
		namespace NET
		{
			enum opcode_e
			{
				opcode_UNDEFINED,
				C_AUTH,
				S_AUTH,
				C_QUERY,
				S_QUERY,
				S_REPLY,
				C_REPLY
			};

			enum statuscode_e
			{
				statuscode_UNDEFINED,
				OK,
				ACK,
				NACK
			};

			class packet_fingerprint
			{
			public:
				int packetid = 0;
				time_t sent_time = 0;
				std::uint32_t crc = 0;

				template<class Archive>
				void serialize(Archive& ar)
				{
					ar(packetid, sent_time, crc);
				}
			};
			
			class packet_base
			{
			public:
				std::string packet_handle = "UNDEFINED";
				opcode_e opcode = opcode_e::opcode_UNDEFINED;
				statuscode_e statuscode = statuscode_e::statuscode_UNDEFINED;
				packet_fingerprint packet_info;

				template<class Archive>
				void serialize(Archive& ar)
				{
					ar(packet_info, opcode, statuscode, packet_handle);
				}

			};

			class single_episode_packet : public packet_base
			{
			public:
				std::string packet_handle = "jt_single_episode_packet";
				JATAPT::COMMON::J_EP ep;
				
				template<class Archive>
				void serialize(Archive& ar)
				{
					packet_base::serialize(ar);
					ar(ep);
				}
			};

		}
	}
}