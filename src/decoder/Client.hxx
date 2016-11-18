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

#ifndef MPD_DECODER_CLIENT_HXX
#define MPD_DECODER_CLIENT_HXX

#include "check.h"
#include "DecoderCommand.hxx"
#include "Chrono.hxx"
#include "Compiler.h"

#include <stdint.h>

struct AudioFormat;

/**
 * An interface between the decoder plugin and the MPD core.
 */
class DecoderClient {
public:
	/**
	 * Notify the client that it has finished initialization and
	 * that it has read the song's meta data.
	 *
	 * @param audio_format the audio format which is going to be
	 * sent to SubmitData()
	 * @param seekable true if the song is seekable
	 * @param duration the total duration of this song; negative if
	 * unknown
	 */
	virtual void Ready(AudioFormat audio_format,
			   bool seekable, SignedSongTime duration) = 0;

	/**
	 * Determines the pending decoder command.
	 *
	 * @return the current command, or DecoderCommand::NONE if there is no
	 * command pending
	 */
	gcc_pure
	virtual DecoderCommand GetCommand() = 0;

	/**
	 * Called by the decoder when it has performed the requested command
	 * (dc->command).  This function resets dc->command and wakes up the
	 * player thread.
	 */
	virtual void CommandFinished() = 0;

	/**
	 * Call this when you have received the DecoderCommand::SEEK command.
	 *
	 * @return the destination position for the seek in milliseconds
	 */
	gcc_pure
	virtual SongTime GetSeekTime() = 0;

	/**
	 * Call this when you have received the DecoderCommand::SEEK command.
	 *
	 * @return the destination position for the seek in frames
	 */
	gcc_pure
	virtual uint64_t GetSeekFrame() = 0;

	/**
	 * Call this instead of CommandFinished() when seeking has
	 * failed.
	 */
	virtual void SeekError() = 0;
};

#endif
