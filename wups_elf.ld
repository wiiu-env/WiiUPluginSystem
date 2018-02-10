SECTIONS {
	.wups.meta : {
        wups_meta = .;
		*(.wups.meta*)
	}
	.wups.load : {
        wups_load = .;
		*(.wups.load*)
	}
	.wups.hooks : {
        wups_hooks = .;
		*(.wups.hooks*)
	}
}