#!/usr/bin/python
#Dateiname: Adjektiven.py

adjdiff = raw_input("Adjective Differentiation Point: ")
adjfirst = raw_input("Letters Before Diff Point: ")
adjfull = raw_input("Full Adjective Form: ")
adjreg = raw_input("Regular: ")
adjcomp = raw_input("Comparative: ")
adjsup = raw_input("Superlative:  ")

prompt = raw_input("Write? ")

if prompt == "y":

	fileHandle = open ( 'Adjektiven.txt', 'a' )
	fileHandle.write ( '    <pardef n="' + adjdiff + '__adj">\n' )
	fileHandle.write ( '      <e>\n' )
	fileHandle.write ( '       <p>\n' )
        fileHandle.close()

	if adjreg == adjfirst:

		fileHandle = open ( 'Adjektiven.txt', 'a')
		fileHandle.write ( '        <l/>\n' )
		fileHandle.write ( '        <r><s n="adj"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '      <e>\n' )
		fileHandle.write ( '       <p>\n' )
		fileHandle.write ( '        <l>' + adjcomp + '</l>\n' )
		fileHandle.write ( '        <r><s n="adj"/><s n="comp"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '      <e>\n' )
		fileHandle.write ( '       <p>\n' )
		fileHandle.write ( '        <l>' + adjsup + '</l>\n' )
		fileHandle.write ( '        <r><s n="adj"/><s n="sup"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '    </pardef>\n\n' )
		fileHandle.close()

		fileHandle = open ( 'Adjektiven_Bilingual.txt', 'a')
		fileHandle.write ('    <e lm="' + adjfull + '<i>' + adjfirst + '</i><par n=' + adjdiff + '__adj"/></e>' )
		fileHandle.close()

	else:

		fileHandle = open ( 'Adjektiven.txt', 'a')
		fileHandle.write ( '        <l>' + adjreg + '</l>\n' )
		fileHandle.write ( '        <r><s n="adj"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '      <e>\n' )
		fileHandle.write ( '       <p>\n' )
		fileHandle.write ( '        <l>' + adjcomp + '</l>\n' )
		fileHandle.write ( '        <r><s n="adj"/><s n="comp"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '      <e>\n' )
		fileHandle.write ( '       <p>\n' )
		fileHandle.write ( '        <l>' + adjsup + '</l>\n' )
		fileHandle.write ( '        <r><s n="adj"/><s n="sup"/></r>\n' )
		fileHandle.write ( '       </p>\n' )
		fileHandle.write ( '      </e>\n' )
		fileHandle.write ( '    </pardef>\n\n' )
		fileHandle.close()

		fileHandle = open ( 'Adjektiven_Bilingual.txt', 'a')
		fileHandle.write ('    <e lm="' + adjfull + '<i>' + adjfirst + '</i><par n=' + adjdiff + '__adj"/></e>' )
		fileHandle.close()

else:
	print "Then why the hell would you add the information to begin with?"