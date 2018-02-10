SECTIONS {
	.wups.meta : {
		*(.wups.meta*)
	}
	.wups.load : {
		*(.wups.load*)
	}
	.text : {
		*(.text*)
	}
	.sdata : {
		*(.sdata*)
	}.data : {
		*(.data*)
	}
	.rodata : {
		*(.rodata*)
	}
	.got2 : {
		*(.got2*)
	}
	.sbss : {
		*(.sbss*)
	}
	.bss : {
		*(.bss*)
	}
	/DISCARD/ : {
		*(*)
	}
}