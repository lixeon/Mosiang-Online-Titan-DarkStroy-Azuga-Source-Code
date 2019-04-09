<%@ Control Language="c#" AutoEventWireup="false" Codebehind="viewnews.ascx.cs" Inherits="web.control.viewnews" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<TABLE cellSpacing="0" cellPadding="0" width="96%" border="0">
	<TR>
		<TD height="30"></TD>
		<TD align="center" height="30">
			<span id="AscxNewsView1_L_Title" style="FONT-WEIGHT:bold;"><%=strtitle%></span></TD>
	</TR>
	<TR height="25">
		<TD></TD>
		<TD>
			<P align="center">
				°l±í•rég£º <span id="AscxNewsView1_L_Date"><%=stradddate%></span>&nbsp;&nbsp; 
				°l±íÈË£º <span id="AscxNewsView1_Label1"><%=strauthor%></span></P>
		</TD>
	</TR>
	<TR>
		<TD></TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat"
			vAlign="top" height="200">&nbsp;&nbsp; <span id="AscxNewsView1_L_Body"><%=strcontent%></span>
		</TD>
	</TR>
</TABLE>
