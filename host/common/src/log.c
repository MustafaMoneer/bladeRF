/*
 * This file is part of the bladeRF project:
 *   http://www.github.com/nuand/bladeRF
 *
 * Copyright (c) 2013 Nuand LLC.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifdef LOGGING_ENABLED
#include <log.h>
#if !defined(WIN32) && !defined(__CYGWIN__) && defined(LOG_SYSLOG_ENABLED)
#include <syslog.h>
#endif
#include <stdio.h>
#include <stdarg.h>

static bladerf_log_level filter_level = BLADERF_LOG_LEVEL_INFO;

void log_write(bladerf_log_level level, const char *format, ...)
{
    /* Only process this message if its level exceeds the current threshold */
    if (level >= filter_level)
    {
        va_list args;

        /* Write the log message */
        va_start(args, format);
#if defined(WIN32) || defined(__CYGWIN__)
        vfprintf(stderr, format, args);
#else
#  if defined (LOG_SYSLOG_ENABLED)
        {
            int syslog_level;

            switch (level) {
                case BLADERF_LOG_LEVEL_VERBOSE:
                case BLADERF_LOG_LEVEL_DEBUG:
                    syslog_level = LOG_DEBUG;
                    break;

                case BLADERF_LOG_LEVEL_INFO:
                    syslog_level = LOG_INFO;
                    break;

                case BLADERF_LOG_LEVEL_WARNING:
                    syslog_level = LOG_WARNING;
                    break;

                case BLADERF_LOG_LEVEL_ERROR:
                    syslog_level = LOG_ERR;
                    break;

                case BLADERF_LOG_LEVEL_CRITICAL:
                    syslog_level = LOG_CRIT;
                    break;

                default:
                    /* Shouldn't be used, so just route it to a low level */
                    syslog_level = LOG_DEBUG;
                    break;
            }

            vsyslog(syslog_level | LOG_USER, format, args);
        }
#  else
        vfprintf(stderr, format, args);
#  endif
#endif
        va_end(args);
    }
}

void log_set_verbosity(bladerf_log_level level)
{
    filter_level = level;
}
#endif
