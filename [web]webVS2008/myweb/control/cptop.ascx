<%@ Control Language="c#" AutoEventWireup="false" Codebehind="cptop.ascx.cs" Inherits="web.control.cptop" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table width="780" border="0" cellspacing="1" cellpadding="3">
	<tr>
		<td rowspan="3" width="265"><a href="cpserverinfo.aspx"><img src="../images/admin/cplogo.gif" border="0"></a></td>
		<td height="30" colspan="3">
			<div align="left"><font color="#ff6633"><b>GodMH Web System</b></font><b>[Version:1.1
					Build 0510]</b></div>
		</td>
	</tr>
	<tr>
		<td height="1" bgcolor="#000000" colspan="3"></td>
		<td height="1" width="4"></td>
	</tr>
	<tr>
		<td width="184" style="WIDTH: 184px"><b>®”Ç°µÇê‘ÓÃ‘ô:</b><font color="#ff0000"><%=Session["admin_name"]%></font></td>
		<td height="30">
			<div align="right"></div>
		</td>
		<td height="30" width="4">&nbsp;</td>
	</tr>
</table>
