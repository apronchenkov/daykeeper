#ifndef __tables_h__
#define __tables_h__

/************************************************************************/
#define partition_header_size (0x200)

#define ph_oem_id (0x003)
#define ph_bytes_per_sector (0x00b)
#define ph_sectors_per_cluster (0x00d)
#define ph_total_sectors (0x028)
#define ph_lcn_of_mft (0x030)
#define ph_lcn_of_mft_mirr (0x038)
#define ph_clusters_per_mft_record (0x040)
#define ph_clusters_per_index_block (0x044)

/************************************************************************/
#define mft_record_size (0x030)

#define	mr_magic_number (0x000)
#define	mr_offset_to_update_sequence (0x004)
#define mr_size_of_update_sequence (0x006)
#define	mr_logfile_sequence_number (0x008)
#define	mr_sequence_number (0x010)
#define	mr_hard_link_count (0x012)
#define	mr_offset_to_first_attribute (0x014)
#define	mr_flags (0x016)
#define mr_reference_to_base_mft_record (0x020)
#define mr_next_attribute_ID (0x028)

#define mr_flag_used (0x001)
#define mr_flag_directory (0x002)
/************************************************************************/
#define mft_reference_record_number(mftref) (0xffffffffffffLL & (mftref))
#define mft_reference_sequence_number(mftref) ((mftref) >> 48)
/************************************************************************/
#define attribute_resident_header_size (0x018)
#define attribute_non_resident_header_size (0x038)

#define ah_attribute_type (0x000)
#define ah_attribute_record_lenght (0x004)
#define ah_non_resident_flag (0x008)
#define ah_name_lenght (0x009)
#define ah_offset_to_name (0x00A)
#define ah_flags (0x00C)
#define ah_ID (0x00E)
#define ahr_lenght_of_attribute (0x010)
#define ahr_offset_to_attribute (0x014)
#define	ahr_indexed_flag (0x016)
#define	ahnr_starting_vcn (0x010)
#define	ahnr_last_vcn (0x018)
#define	ahnr_offset_to_data_runs (0x020)
#define ahnr_compression_unit_size (0x022)
#define ahnr_real_size_of_attribute (0x030)

#define ah_flag_normal (0x0000)
#define ah_flag_compress (0x0001)
#define ah_flag_encrypted (0x4000)
#define ah_flag_sparse (0x8000)
/************************************************************************/

#define attrib_ATTRIBUTE_LIST (0x00000020)
#define attrib_DATA (0x00000080)
#define attrib_FILE_NAME (0x00000030)
#define attrib_end_of_attributes (0xffffffff)

#endif //__tables_h__
