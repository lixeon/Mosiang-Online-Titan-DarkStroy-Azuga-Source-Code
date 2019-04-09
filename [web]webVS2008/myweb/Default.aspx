<%@ Page language="c#" Codebehind="Default.aspx.cs" AutoEventWireup="false" Inherits="web._Default"%>
<%@ Register TagPrefix="uc1" TagName="agentlist" Src="control/newagentlist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="menudownloadlist" Src="control/menudownloadlist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="register" Src="control/register.ascx" %>
<%@ Register TagPrefix="uc1" TagName="login" Src="control/login.ascx" %>
<%@ Register TagPrefix="uc1" TagName="newstypelist" Src="control/menunewstypelist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="smalltop" Src="control/smalltop.ascx" %>
<%@ Register TagPrefix="uc1" TagName="foot" Src="control/foot.ascx" %>
<%@ Register TagPrefix="uc1" TagName="newslist" Src="control/menunewslist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="top" Src="control/top.ascx" %>
<%@ Register TagPrefix="uc1" TagName="memberfun" Src="control/memberfun.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>
			<%=Application["web.title"]%>
		</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content='<%=Application["web.description"]%>' name="description">
		<meta content='<%=Application["web.keywords"]%>' name="keywords">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<LINK href="images/style.css" type="text/css" rel="stylesheet">
		<link rel="shortcut icon" href="web.ico">
		<link rel="Bookmark" href="web.ico"> 
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<uc1:top id="Top1" runat="server"></uc1:top>
			<table cellSpacing="0" cellPadding="0" width="956" align="left" border="0">
				<tr>
					<td vAlign="top" width="207" background=images/bg_logincol.gif>
						<table cellSpacing="2" cellPadding="1" width="207">
							<tr>
								<td>
									<uc1:login id="Login1" runat="server"></uc1:login>
								</td>
							</tr>
							<tr>
								<td align="center"><a href="download.aspx"><IMG alt="üc“ôÏÂÝd¿Í‘ô¶ËÅcµÇê‘Æ÷" src="images/col_download.gif"></a></td>
							</tr>
							<TR>
								<TD align="center"><a href="<%=Application["web.forum"]%>" target="_blank"><IMG src="images/col_bbs.gif"></a></TD>
							</TR>
							<TR>
								<TD align=middle><A href="shop.aspx" target=_blank><IMG src="images/col_buy.gif"></A></TD>
							</TR>
        
							<tr>
								<td><uc1:memberfun id="Memberfun1" runat="server"></uc1:memberfun></td>
							</tr>
							<TR>
								<TD><uc1:newstypelist id="Newstypelist1" runat="server"></uc1:newstypelist></TD>
							</TR>
						</table>
					</td>
					<td vAlign="top" width="488">
						<table>
							<tr>
								<td align="center" width="488" background="images/top_line_notice.jpg" height="46"></td>
							</tr>
							<tr>
								<td vAlign="top" width="488" background="images/bg_logincol.gif" height="400"><%=Application["notice"]%><FONT face="ËÎÌå"></FONT></td>
							</tr>
							<TR>
								<td align="center" width="488" background="images/col_bg.gif" height="32">
								</td>
							<tr>
								<td vAlign="top" background="images/bg_logincol.gif"><uc1:newslist id="Newslist1" runat="server"></uc1:newslist></td>
							</tr>
							<TR>
								<td align="center" width="488" background="images/download.gif" height="32">
								</td>
							<tr>
								<td vAlign="top" background="images/bg_logincol.gif">
									<uc1:menudownloadlist id="Menudownloadlist1" runat="server"></uc1:menudownloadlist></td>
							</tr>
						</table>
					</td>
					      <TD width="283" vAlign=top background="images/bg_logincol.gif" align=center> <table width="283" border="0" cellspacing="0" cellpadding="0">
						<uc1:agentlist id="Agentlist1" runat="server"></uc1:agentlist>
						<uc1:smalltop id="Smalltop1" runat="server"></uc1:smalltop>


				<TR>
					<TD colSpan="10">
						<uc1:foot id="Foot1" runat="server"></uc1:foot></TD>

				</TR>
			</table>
		</form>
	</body>
</HTML>
