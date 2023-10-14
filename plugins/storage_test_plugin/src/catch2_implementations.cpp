#include <iostream>
#include <stdint.h>
#include <vector>

namespace Catch {

#define OSConsoleWriteEx ((void (*)(uint32_t, uint32_t, const char *, size_t))(0x101C400 + 0x10cf0))

    bool WUMSLogWrite(const char *str, size_t size) {
        OSConsoleWriteEx(0xFFFFFFFF, 0xFFFFFFFF, str, size);
        return true;
    }

    // Logs to OSConsole
    class CustomLogStream : public std::ostream {
    private:
        class LogStreambuf : public std::streambuf {
        public:
            LogStreambuf() {
                // Initialize the buffer with a small size (adjust as needed)
                buffer.resize(512);
                setp(buffer.data(), buffer.data() + buffer.size());
            }

            ~LogStreambuf() {
                sync();
            }

        protected:
            int overflow(int c) override {
                if (c != EOF) {
                    *pptr() = static_cast<char>(c);
                    pbump(1);
                }
                if (flushBuffer() == EOF) {
                    return EOF;
                }
                return c;
            }

            int sync() override {
                return flushBuffer();
            }

            int flushBuffer() {
                if (pbase() != pptr()) {
                    size_t size = pptr() - pbase();
                    if (!WUMSLogWrite(pbase(), size)) {
                        return EOF;
                    }
                    setp(pbase(), epptr());
                }
                return 0;
            }

        private:
            std::vector<char> buffer;
        };

        LogStreambuf logStreambuf;

    public:
        CustomLogStream() : std::ostream(&logStreambuf) {
        }
    };

    CustomLogStream customlog;

    std::ostream &cout() { return customlog; }

    std::ostream &cerr() { return customlog; }

    std::ostream &clog() { return customlog; }
} // namespace Catch