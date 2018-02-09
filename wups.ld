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
	.data : {
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