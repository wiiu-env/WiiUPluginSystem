SECTIONS
{
   .wups.meta : {
	  *(.wups.meta*)
      KEEP(*(.wups.meta*))
   } > datamem
   
   .wups.load : {
	  *(.wups.load*)
      KEEP(*(.wups.load*))
   } > datamem
   
   .wups.hooks : {
	  *(.wups.hooks*)
      KEEP(*(.wups.hooks*))
   } > datamem
}