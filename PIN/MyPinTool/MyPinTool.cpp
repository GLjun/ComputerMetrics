
/*! @file
 *  This is an example of the PIN tool that demonstrates some basic PIN APIs 
 *  and could serve as the starting point for developing your first PIN tool
 */

#include "pin.H"
#include <iostream>
#include <fstream>
#include <map>

#define PT 0
#define PTN 1
#define PNT 2
#define PN 3

class predictor
{
private:
    int pre;

public:
    bool taken;
    ADDRINT targetPC;
    predictor()
    {
        pre = PT;
        taken = true;
        targetPC = 0;   
    }
    void update(bool takenflag, ADDRINT tpc)
    {
	if(targetPC == 0)
	    targetPC = tpc;

        if(takenflag)
        {
            if(pre == PTN)
                pre = PT;
            else if(pre == PNT)
            {
                pre = PTN;
                taken = true;
		targetPC = tpc;
            }
            else if(pre == PN)
                pre = PNT;
        }
        else
        {
            if(pre == PT)
                pre = PTN;
            else if(pre == PTN)
            {
                pre = PNT;
                taken = false;
		targetPC = tpc;
            }
            else if(pre == PNT)
                pre = PN;
        }
    }
};

class BranchPredictor
{
public:
    predictor* getPredictor(ADDRINT pc)
    {
        if(addrToPre.find(pc) != addrToPre.end())
            return addrToPre[pc];
        else
        {
            predictor *p = new predictor();
            addrToPre.insert(std::pair<ADDRINT, predictor*>(pc, p));
            return p;
        }
    }

    void inc_predict_count() { this->count_predict ++; }
    void inc_misspredict_count() { this->count_misspredict ++; }
    void inc_target_predict_count() { this->count_predict_target ++; }
    void inc_target_misspredict_count() { this->count_misspredict_target ++; }

    long long predict_count() { return this->count_predict; }
    long long misspredict_count() { return this->count_misspredict; }
    long long target_predict_count() { return this->count_predict_target; }
    long long target_misspredict_count() { return this->count_misspredict_target; }

    BranchPredictor():count_misspredict(0), count_predict(0), count_predict_target(0), count_misspredict_target(0) {}

    ~BranchPredictor()
    {
        for(std::map<ADDRINT, predictor*>::iterator it = addrToPre.begin(); it != addrToPre.end(); it ++)
            if(it->second != NULL)
                delete it->second;
    }
    
private:
    std::map<ADDRINT, predictor*> addrToPre;
    long long count_misspredict;
    long long count_predict;
    long long count_predict_target;
    long long count_misspredict_target;
};

BranchPredictor myBP;

std::ostream * out = &cerr;

/* ===================================================================== */
// Command line switches
/* ===================================================================== */
KNOB<string> KnobOutputFile(KNOB_MODE_WRITEONCE,  "pintool",
    "o", "", "specify file name for MyPinTool output");

KNOB<BOOL>   KnobCount(KNOB_MODE_WRITEONCE,  "pintool",
    "count", "1", "count instructions, basic blocks and threads in the application");

KNOB<int>   KnobInt(KNOB_MODE_WRITEONCE,  "pintool",
    "int", "10", "count instructions, basic blocks and threads in the application");

/*!
 *  Print out help message.
 */
INT32 Usage()
{
    cerr << "This tool prints out the number of dynamically executed " << endl <<
            "instructions, basic blocks and threads in the application." << endl << endl;

    cerr << KNOB_BASE::StringKnobSummary() << endl;
    return -1;
}

VOID ProcessBranch(ADDRINT pc, ADDRINT targetPC, bool BrTaken)
{
    myBP.inc_predict_count();
    myBP.inc_target_predict_count();
    predictor *p = myBP.getPredictor(pc);
    if(p->taken != BrTaken)
        myBP.inc_misspredict_count();
    if(p->targetPC != targetPC)
        myBP.inc_target_misspredict_count();

    p->update(BrTaken, targetPC);

}

VOID Instruction(INS ins, VOID *v)
{
    if(INS_IsBranch(ins) && INS_HasFallThrough(ins))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)ProcessBranch, 
                IARG_ADDRINT, INS_Address(ins), 
                IARG_ADDRINT, INS_DirectBranchOrCallTargetAddress(ins),
                IARG_BRANCH_TAKEN, IARG_END);
    }
}


/*!
 * Print out analysis results.
 * This function is called when the application exits.
 * @param[in]   code            exit code of the application
 * @param[in]   v               value specified by the tool in the 
 *                              PIN_AddFiniFunction function call
 */
VOID Fini(INT32 code, VOID *v)
{
    *out <<  "===============================================" << endl;
    *out <<  "MyPinTool analysis results: " << endl;
    *out <<  "Number of Branch Predict: " << myBP.predict_count()  << endl;
    *out <<  "Number of Branch Predict Misspredict: " << myBP.misspredict_count()  << endl;
    *out <<  "Branch correct predict rate: " << 100.0 * (1.0 - 1.0*myBP.misspredict_count()/ myBP.predict_count()) << " %" << endl;
    *out <<  "Number of Branch Predict Target: " << myBP.target_predict_count()  << endl;
    *out <<  "Number of Branch Predict Target Misspredict: " << myBP.target_misspredict_count()  << endl;
    *out <<  "target correct predict rate: " << 100.0 * (1.0 - 1.0*myBP.target_misspredict_count()/ myBP.target_predict_count()) << " %" << endl;
    *out <<  "===============================================" << endl;
}

/*!
 * The main procedure of the tool.
 * This function is called when the application image is loaded but not yet started.
 * @param[in]   argc            total number of elements in the argv array
 * @param[in]   argv            array of command line arguments, 
 *                              including pin -t <toolname> -- ...
 */
int main(int argc, char *argv[])
{
    // Initialize PIN library. Print help message if -h(elp) is specified
    // in the command line or the command line is invalid 
    if( PIN_Init(argc,argv) )
    {
        return Usage();
    }
    
    string fileName = KnobOutputFile.Value();

    if (!fileName.empty()) { out = new std::ofstream(fileName.c_str());}
    else { out = &cerr; }
    
    INS_AddInstrumentFunction(Instruction, 0);

    PIN_AddFiniFunction(Fini, 0);
    
    cerr <<  "===============================================" << endl;
    cerr <<  "This application is instrumented by MyPinTool" << endl;
    if (!KnobOutputFile.Value().empty()) 
    {
        cerr << "See file " << KnobOutputFile.Value() << " for analysis results" << endl;
    }
    cerr <<  "===============================================" << endl;

    // Start the program, never returns
    PIN_StartProgram();
    
    return 0;
}

/* ===================================================================== */
/* eof */
/* ===================================================================== */
