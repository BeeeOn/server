<?xml version="1.0" encoding="utf-8"?>
<x:stylesheet xmlns:x="http://www.w3.org/1999/XSL/Transform"
		version="1.0">

	<x:param name="verbosity" select="0" />
	<x:param name="hide.unused" select="true()" />

	<x:variable name="ini.database.backend">sqlite</x:variable>
	<x:variable name="ini.database.dialect">sqlite</x:variable>

	<x:param name="groups">
		<!-- colors: http://www.graphviz.org/doc/info/colors.html -->
		<group pattern="XmlRequestHandlerFactory" />
		<group pattern="UIServerModule" />
		<group pattern="Service" fillcolor="coral" />
		<group pattern="RandomProvider" fillcolor="mintcream" />
		<group pattern="SQL" fillcolor="blue" fontcolor="white" />
		<group pattern="Mock" fillcolor="mediumpurple1" />
		<group pattern="Dao" fillcolor="deepskyblue1" />
		<group pattern="RPC" fillcolor="darkorchid1" />
		<group pattern="Handler" fillcolor="lawngreen" />
		<group pattern="Policy" fillcolor="gray24" fontcolor="white" />
		<group pattern="AuthProvider" fillcolor="gray24" fontcolor="white" />
		<group pattern="Session" fillcolor="grey24" fontcolor="white" />
		<group pattern="SSL" fillcolor="orange" />
		<group pattern="Notification" fillcolor="lightyellow" />
		<group pattern="Observer" fillcolor="lightyellow" />
		<group pattern="Sender" fillcolor="lightyellow" />
		<group pattern="Template" fillcolor="darkgreen" fontcolor="white" />
	</x:param>

</x:stylesheet>
