#pragma pack(1)
#define MaxProcessNum 5
// register table
struct regs {
	short ax;
	short bx;
	short cx;
	short dx;
	short si;
	short di;
	short bp;
	short es;
	short ds;
	short ss;
	short sp;
	short ip;
	short cs;
	short flags;
};
// structure for process infomation
// the first pcb is reserved for the kernel
struct pcb {
	struct regs regTable;	// the old register table
	short pid;				// index of the process
	char pname[10];			// name of the program
	char pstate;			// 0 empty / 1 active
};
struct pcb pcbList[MaxProcessNum];
// pointer to the current process
struct pcb* pcbPtr;
short nowPid;
// number of running processes
short proCount;
// signal after programs are ready
short ready;

// c function to select the next available process
void schedule() {
	// only the kernel is running 
	if(proCount == 1) {
		nowPid = 0;
	} else {
		do {
			// in the kernel, loading not finished
			if(!nowPid && !ready) break;
			// in apps or, the kernel is ready
			nowPid = nowPid == MaxProcessNum - 1 ? 1 : nowPid + 1;
		}while(!pcbList[nowPid].pstate);
	}
	pcbPtr = &pcbList[nowPid];
}