#pragma pack(1)
struct pcb {
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
struct pcb Pcb = {};
struct pcb* pcbPtr = &Pcb;