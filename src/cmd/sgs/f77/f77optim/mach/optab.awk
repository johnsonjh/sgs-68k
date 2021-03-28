
#
#	optab.awk	7.1	
#
#	optab.awk:	2.1 83/07/08

#ident	"@(#)f77/f77optim:optab.awk	25.1"

BEGIN		{ OFS = ""; FS = "\t"; ORS = ""; first = "t"; 
		  opnum = 10; # low numbers 0..9 reserved for optim.c use
			      # to flag special opcode types
		  print "#define OP_MIN\t\t",opnum, \
			"\t/* Min. opcode no. */\n\n" >"ops.tail";
		}
$1 == "/*"	{ print; print "\n"; next }
$1=="#ifdef" && $2=="M68020"&&p!="M68020",$1=="#endif" {next} # ignore to #endif
$1=="#ifdef" && $2=="M68040"&&p!="M68040",$1=="#endif" {next} # ignore to #endif
$1=="#ifdef" && $2=="m68k"&&  f!="m68k",  $1=="#endif" {next} # ignore to #endif
$1 == "#ifdef"	{next} # don't print conditional inclusion lines
$1 == "#endif"	{next} #                  "
first == "f"	{ print "," }
first == "t"	{
		 print "{"; comment = "" }
		 #beginning of line processing
length(comment) != 0 \
		{ print "\t/* ", comment, " */"; comment = "" }
		# add an opcode equate to ops.tail
		  { print "#define ", $2, "      \t", opnum, \
		        "\t/* ",$1," */\n" >>"ops.tail";
		  opnum++;
		  print "\n" ;
		  print "    { \"", $1, "\",\t" ;
		  print $2, ",\t" ;
		  print $3, ",\t", $4, ",\t{", $5, "},\t", $6, ",\t" $7, \
			",\t", $8, " }" ;
		  first = "f" }
NF > 8		{ comment = $9 }
END		{ print "\n};\n";
		  opnum--;
		  print "\n#define OP_MAX\t\t",opnum, \
			"\t/* Max. opcode no. in the 'op' table */\n\n"\
			>>"ops.tail";
		  opnum++;
		  print "/* The following two directives need opcode numbers",\
			",    */\n" >>"ops.tail";
		  print "/* but they do not lie in the normal opcode table ",\
			"'op'. */\n\n" >>"ops.tail";
		  print "#define LABEL\t\t",opnum, \
			"\t/* Label directive opcode num. */\n\n" >>"ops.tail";
		  opnum++;
		  print "#define SWITCH\t\t",opnum, \
			"\t/* Switch directive opcode num. */\n\n" >>"ops.tail";
		}
