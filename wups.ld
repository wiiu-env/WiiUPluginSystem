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
	.bss : {
		*(.bss*)
	}
	/DISCARD/ : {
		*(*)
	}
}