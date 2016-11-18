/*
 * Copyright 2003-2016 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/*! \file
 * \brief The MPD Decoder API
 *
 * This is the public API which is used by decoder plugins to
 * communicate with the mpd core.
 */

#ifndef MPD_DECODER_API_HXX
#define MPD_DECODER_API_HXX

// IWYU pragma: begin_exports

#include "check.h"
#include "Client.hxx"
#include "input/Ptr.hxx"
#include "DecoderCommand.hxx"
#include "DecoderPlugin.hxx"
#include "ReplayGainInfo.hxx"
#include "tag/Tag.hxx"
#include "AudioFormat.hxx"
#include "MixRampInfo.hxx"
#include "config/Block.hxx"
#include "Chrono.hxx"

// IWYU pragma: end_exports

#include <stdint.h>

class DecoderClient;

/**
 * Throw an instance of this class to stop decoding the current song
 * (successfully).  It can be used to jump out of all of a decoder's
 * stack frames.
 */
class StopDecoder {};

/**
 * Open a new #InputStream and wait until it's ready.
 *
 * Throws #StopDecoder if DecoderCommand::STOP was received.
 *
 * Throws std::runtime_error on error.
 */
InputStreamPtr
decoder_open_uri(DecoderClient &decoder, const char *uri);

/**
 * Blocking read from the input stream.
 *
 * @param decoder the decoder object
 * @param is the input stream to read from
 * @param buffer the destination buffer
 * @param length the maximum number of bytes to read
 * @return the number of bytes read, or 0 if one of the following
 * occurs: end of file; error; command (like SEEK or STOP).
 */
size_t
decoder_read(DecoderClient *decoder, InputStream &is,
	     void *buffer, size_t length);

static inline size_t
decoder_read(DecoderClient &decoder, InputStream &is,
	     void *buffer, size_t length)
{
	return decoder_read(&decoder, is, buffer, length);
}

/**
 * Blocking read from the input stream.  Attempts to fill the buffer
 * completely; there is no partial result.
 *
 * @return true on success, false on error or command or not enough
 * data
 */
bool
decoder_read_full(DecoderClient *decoder, InputStream &is,
		  void *buffer, size_t size);

/**
 * Skip data on the #InputStream.
 *
 * @return true on success, false on error or command
 */
bool
decoder_skip(DecoderClient *decoder, InputStream &is, size_t size);

/**
 * Sets the time stamp for the next data chunk [seconds].  The MPD
 * core automatically counts it up, and a decoder plugin only needs to
 * use this function if it thinks that adding to the time stamp based
 * on the buffer size won't work.
 */
void
decoder_timestamp(DecoderClient &decoder, double t);

/**
 * This function is called by the decoder plugin when it has
 * successfully decoded block of input data.
 *
 * @param decoder the decoder object
 * @param is an input stream which is buffering while we are waiting
 * for the player
 * @param data the source buffer
 * @param length the number of bytes in the buffer
 * @return the current command, or DecoderCommand::NONE if there is no
 * command pending
 */
DecoderCommand
decoder_data(DecoderClient &decoder, InputStream *is,
	     const void *data, size_t length,
	     uint16_t kbit_rate);

static inline DecoderCommand
decoder_data(DecoderClient &decoder, InputStream &is,
	     const void *data, size_t length,
	     uint16_t kbit_rate)
{
	return decoder_data(decoder, &is, data, length, kbit_rate);
}

/**
 * This function is called by the decoder plugin when it has
 * successfully decoded a tag.
 *
 * @param is an input stream which is buffering while we are waiting
 * for the player
 * @param tag the tag to send
 * @return the current command, or DecoderCommand::NONE if there is no
 * command pending
 */
DecoderCommand
decoder_tag(DecoderClient &decoder, InputStream *is, Tag &&tag);

static inline DecoderCommand
decoder_tag(DecoderClient &decoder, InputStream &is, Tag &&tag)
{
	return decoder_tag(decoder, &is, std::move(tag));
}

/**
 * Set replay gain values for the following chunks.
 *
 * @param replay_gain_info the replay_gain_info object; may be nullptr
 * to invalidate the previous replay gain values
 */
void
decoder_replay_gain(DecoderClient &decoder,
		    const ReplayGainInfo *replay_gain_info);

/**
 * Store MixRamp tags.
 */
void
decoder_mixramp(DecoderClient &decoder, MixRampInfo &&mix_ramp);

#endif
