<%@ Register TagPrefix="uc1" TagName="viewnews" Src="control/viewnews.ascx" %>
<%@ Register TagPrefix="uc1" TagName="menudownloadlist" Src="control/menudownloadlist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="newstypelist" Src="control/menunewstypelist.ascx" %>
<%@ Register TagPrefix="uc1" TagName="foot" Src="control/foot.ascx" %>
<%@ Register TagPrefix="uc1" TagName="top" Src="control/top.ascx" %>
<%@ Register TagPrefix="uc1" TagName="memberfun" Src="control/memberfun.ascx" %>
<%@ Page language="c#" Codebehind="download.aspx.cs" AutoEventWireup="false" Inherits="web.download" %>
<%@ Register TagPrefix="uc1" TagName="download" Src="control/download.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>
			<%=Application["web.title"]%>
		</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<meta content='<%=Application["web.description"]%>' name="description">
		<meta content='<%=Application["web.keywords"]%>' name="keywords">
		<LINK href="images/style.css" type="text/css" rel="stylesheet">
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<uc1:top id="Top1" runat="server"></uc1:top>
			<table cellSpacing="0" cellPadding="0" width="956" align="left" border="0">
				<tr>
					<td vAlign="top" width="207" background=images/bg_logincol.gif>
						<table cellSpacing="2" cellPadding="1" width="207">
							<tr>
								<td align="center"><a href="default.aspx"><IMG alt="回到首頁" src="images/button_homepage.jpg"></a></td>
							</tr>
							<tr>
								<td align="center"><a href="download.aspx"><IMG alt="點擊下載客戶端與登陸器" src="images/col_download.gif"></a></td>
							</tr>
							<tr>
								<td><uc1:memberfun id="Memberfun1" runat="server"></uc1:memberfun></td>
							</tr>
							<TR>
								<TD>
									<uc1:newstypelist id="Newstypelist1" runat="server"></uc1:newstypelist></TD>
							</TR>
						</table>
					</td>
					<td vAlign="top" width="775">
						<table>
							<tr>
								<td align="center" width="775" background="images/top_line_download.jpg" height="46"><FONT face="細明體"></FONT></td>
							</tr>
							<tr>
								<td vAlign="top" width="775" background="images/bg_logincol.gif" height="600" align="center">
									<uc1:download id="Download1" runat="server"></uc1:download></td>
							</tr>
						</table>
					</td>
				</tr>
				<tr>
					<td colspan="10"><uc1:foot id="Foot1" runat="server"></uc1:foot>
					</td>
				</tr>
			</table>
		</form>
	</body>
</HTML>
