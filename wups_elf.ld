SECTIONS {
	.wups.meta : {
        bslug_meta = .;
		*(.wups.meta*)
	}
	.wups.load : {
        bslug_load = .;
		*(.wups.load*)
	}
}