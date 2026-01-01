#include "engine/sound/CSample.h"
#include <cstring>

DKSND::CSample::CSample(ISampleDictionary* dictionary) : m_name(""), m_filename("") {
    m_unk0 = 0;
    m_is_3d = FALSE;
    m_dictionary = (CSampleDictionary*)dictionary;
}

DKSND::CSample::~CSample() {
    if (IsValid()) {
        ReleaseSpecific();
    }
}

U32 DKSND::CSample::GetSize() {
    if (m_is_streamed) {
        return 0;
    }

    return m_format.size;
}

std::string* DKSND::CSample::GetName() {
    return &m_name;
}

U32 DKSND::CSample::GetFrequency() {
    return m_format.frequency;
}

DKSND::SSAMPLE_FORMAT DKSND::CSample::GetFormat() {
    return m_format;
}

void DKSND::CSample::SetName(std::string* name) {
    m_name.assign(*name, 0);
}

// May be a fakematch
void DKSND::CSample::SetFormat(SSAMPLE_FORMAT* format) {
#if 1
    m_format.unk0 = format->unk0;
    m_format.unk1 = format->unk1;
    m_format.bitrate = format->bitrate;
    m_format.nb_channels = format->nb_channels;
    m_format.frequency = format->frequency;
    m_format.unk8 = format->unk8;
    m_format.size = format->size;
#else
    m_format = *format;
#endif
}

#pragma dont_inline on
U32 ChangeEndianFormat(U32 value) {
    volatile U8 bytes[4];
    U8 byte0 = bytes[0] = ((value >> 0) & 0xFF);
    U8 byte1 = bytes[1] = ((value >> 8) & 0xFF);
    U8 byte2 = bytes[2] = ((value >> 16) & 0xFF);
    U8 byte3 = bytes[3] = ((value >> 24) & 0xFF);
    return byte0 << 24 | byte1 << 16 | byte2 << 8 | byte3 << 0;
}
#pragma dont_inline off

// Equivalent
U8* DKSND::CSample::LoadSampleFromMemory(U8* a1, U8* a2) {
    std::string str;
    U32 uVar6 = 0;
    for (U8* a1_ = a1; *a1_ != 0; a1_++) {
        uVar6++;
    }
    str = (char*)a1;
    SetName(&str);

    U8* ptr = &a1[uVar6];
    if (uVar6 > 2 && ptr[-2] == '3') {
        Set3D(TRUE);
    }

    memcpy(&m_format, &a1[uVar6], sizeof(SSAMPLE_FORMAT));
    m_format.frequency = ChangeEndianFormat(m_format.frequency);
    m_format.unk8 = ChangeEndianFormat(m_format.unk8);
    m_format.size = ChangeEndianFormat(m_format.size);

    if (!m_dictionary->FindSample(&str, 0)) {
        if (!LoadSpecific(a2 + m_format.unk8)) {
            Invalidate();
        }
    } else {
        Invalidate();
    }

    m_is_streamed = FALSE;

    // str should be destroyed after this is calculated
    return a1 + ((((uVar6 + 16) >> 5) + 1) << 5);
}

void DKSND::CSample::LoadStreamedSampleFromMemory(U8* a1) {
    std::string str;

    U32 uVar6 = 0;
    for (U8* a1_ = a1; *a1_ != 0; a1_++) {
        uVar6++;
    }

    str = (char*)a1;
    a1 += uVar6;
    SetName(&str);

    memcpy(&m_format, a1, sizeof(SSAMPLE_FORMAT));
    m_format.frequency = ChangeEndianFormat(m_format.frequency);
    m_format.unk8 = ChangeEndianFormat(m_format.unk8);
    m_format.size = ChangeEndianFormat(m_format.size);

    m_is_streamed = TRUE;
}

BOOL DKSND::CSample::Is3D() {
    return m_is_3d;
}

void DKSND::CSample::Set3D(BOOL is_3d) {
    m_is_3d = is_3d;
}

BOOL DKSND::CSample::IsStereo() {
    return m_format.nb_channels == 2;
}

BOOL DKSND::CSample::IsStreamed() {
    return m_is_streamed;
}

void DKSND::CSample::SetStreamed(BOOL streamed) {
    m_is_streamed = streamed;
}

U8 DKSND::CSample::GetBitRate() {
    return m_format.bitrate;
}

void DKSND::CSample::SetFullFileName(std::string* filename) {
    m_filename.assign(*filename, 0);
}

std::string* DKSND::CSample::GetFullFileName() {
    return &m_filename;
}
