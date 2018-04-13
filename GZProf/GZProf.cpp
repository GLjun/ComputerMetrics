/*************************************************************************
	> File Name: GZProf.cpp
	> Author: GZProf implementation
	> Mail: 
	> Created Time: 日  4/ 8 21:24:16 2018
 ************************************************************************/

#include <signal.h>
#include <sys/time.h>
#include <unordered_map>
#include <string>
#include <set>
#include <cxxabi.h>
#include <libunwind.h>
#include <fstream>

//#define DEBUG

namespace GZ
{
#define OUTPUT_FILENAME "gzprof.txt"
#define MAX_SYMBOL_LENGTH 256
#define DEFAULT_SAMPLE_FREQUENCY 1000000

#define CHECK(flag, msg) if(!flag) \
            { if(msg)  \
                std::printf("ERROR: check %s\n", msg); \
              exit(-1);\
            }

#define CHECK_WARNING(flag, msg) if(!flag) \
            { if(msg)  \
                std::printf("WARNING: check %s\n", msg); \
            }

extern void sig_prof(int);

class GZProf
{
public:
    GZProf();
    ~GZProf();

    void start();
    bool has_function(char* funcname) { return funcCntMap.find(funcname) != funcCntMap.end(); }
    void add_function(char* funcname) { funcCntMap.emplace(funcname, 1); }
    bool increment_function_count(char* funcname) { funcCntMap[funcname] += 1; }

protected:
    void dump_log_file();
    void init_sigaction();
    void init_frequency();

private:
    int f; //frequency
    std::unordered_map<std::string, int>  funcCntMap;
};

GZProf::GZProf():f(DEFAULT_SAMPLE_FREQUENCY)
{
#ifdef DEBUG
    std::printf("construct GZProf\n");
#endif
}

GZProf::~GZProf()
{
#ifdef DEBUG
    std::printf("destroy GZProf\n");
#endif
    dump_log_file();
}


void GZProf::start()
{
#if defined(DEBUG)
    std::printf("start GZProf\n");
#endif
    init_sigaction();
    init_frequency();
}


void GZProf::init_sigaction()
{
   struct sigaction act; 
   act.sa_handler = sig_prof;
   act.sa_flags = 0;
   sigemptyset(&act.sa_mask);
   sigaction(SIGPROF, &act, NULL);
}

void GZProf::init_frequency()
{

#if defined(SAMPLE_FREQUENCY)
    CHECK(SAMPLE_FREQUENCY >= 0 && SAMPLE_FREQUENCY <= 1e6);
    this->f = SAMPLE_FREQUENCY;
#endif

#if defined(DEBUG)
    std::printf("frequency : %d\n", f);
#endif


    struct itimerval value;
    value.it_value.tv_sec = 0;
    value.it_value.tv_usec = 1e6 / this->f;
    value.it_interval=value.it_value;
    setitimer(ITIMER_PROF, &value, NULL);
}

void GZProf::dump_log_file()
{
#if defined(DEBUG)
    std::printf("begin dump\n");
#endif
    typedef std::pair<std::string, int> sipair;
    auto comp = [](const sipair&e1, const sipair& e2) -> bool  { return e1.second > e2.second; };
    std::set<sipair, decltype(comp)> countSet(comp);

    unsigned int cnt_sum = 0;
    for(auto& x : funcCntMap)
    {
        cnt_sum += x.second;
        countSet.insert({x.first, x.second});
    }

    std::ofstream fout(OUTPUT_FILENAME);
    
    CHECK(fout.is_open(), "ERROR cannot open log file");
    
    for(auto&& x : countSet)
    {
        //std::printf("%s %.2f\%\n", x.first.c_str(), (100.0 * x.second)/cnt_sum);
        fout << x.first << " " << (100.0 * x.second)/cnt_sum << "\n";
    }
    fout.flush();
    fout.close();
#if defined(DEBUG)
    std::printf("finished dump\n");
#endif

}

static GZProf prof;

void gzprof_start()
{
    prof.start();
}

void sig_prof(int signo)
{
    unw_cursor_t cursor;    
    unw_context_t context;

    //initialize cursor
    unw_getcontext(&context);
    unw_init_local(&cursor, &context);

    
    char sym[MAX_SYMBOL_LENGTH] = {0};


    //step over the first frame
    CHECK_WARNING((unw_step(&cursor) > 0), "cannot get symbol name of this frame!");
    //the real frame
    //unwind the first frame
    if(unw_step(&cursor) > 0)
    {
        unw_word_t offset, pc;
        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if(unw_get_proc_name(&cursor, sym, MAX_SYMBOL_LENGTH, &offset) == 0)
        {
            char* nameptr = sym;
            int status;
            char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if(status == 0)
                nameptr = demangled;

            if(prof.has_function(nameptr))
                prof.increment_function_count(nameptr);
            else
                prof.add_function(nameptr);
            
#if defined(DEBUG)
            std::printf(" (%s+0x%lx)\n", nameptr, offset);
#endif
            std::free(demangled);
        }
    }
    else
        std::printf("ERROR : cannot get symbol name of this frame!\n");
    //unwind frames
    /*while(unw_step(&cursor) > 0)
    {
        unw_word_t offset, pc;
        unw_get_reg(&cursor, UNW_REG_IP, &pc);
        if(pc == 0)
            break;
        std::printf("0x%lx", pc);
        if(unw_get_proc_name(&cursor, sym, MAX_SYMBOL_LENGTH, &offset) == 0)
        {
            char* nameptr = sym;
            int status;
            char* demangled = abi::__cxa_demangle(sym, nullptr, nullptr, &status);
            if(status == 0)
                nameptr = demangled;
            std::printf(" (%s+0x%lx)\n", nameptr, offset);
            FREE(demangled);
        }
        else
        {
            std::printf("ERROR : cannot get symbol name of this frame!\n");
        }
    }
    std::printf("\n");*/
}

}


