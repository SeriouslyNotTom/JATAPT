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
				UNDEFINED,
				C_AUTH,
				S_AUTH,
				C_QUERY,
				S_QUERY,
				S_REPLY,
				C_REPLY
			};

			struct packet_fingerprint
			{
				int packetid = 0;
				time_t sent_time = 0;
				std::uint32_t crc = 0;

				template<class Archive>
				void serialize(Archive& ar)
				{
					ar(packetid, sent_time, crc);
				}
			};
			
			struct packet_base
			{
				std::string packet_handle = "undefined";
				opcode_e opcode = opcode_e::UNDEFINED;
				int statuscode = 0;
				packet_fingerprint packet_info;

				template<class Archive>
				void serialize(Archive& ar)
				{
					ar(packet_info, opcode, statuscode, packet_handle);
				}

			};

			struct single_episode_packet : virtual packet_base
			{
				
			};

		}
	}
}