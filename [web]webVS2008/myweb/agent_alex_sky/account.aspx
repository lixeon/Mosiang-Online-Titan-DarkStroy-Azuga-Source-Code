<%@ Page language="c#" Codebehind="account.aspx.cs" AutoEventWireup="false" Inherits="web.agent.account" %>
<%@ Register TagPrefix="uc1" TagName="agentmenu" Src="../control/agentmenu.ascx" %>
<%@ Register TagPrefix="uc1" TagName="agenttop" Src="../control/agenttop.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>agentcp</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
		<SCRIPT language="JavaScript" src="Common/JavaScript/Menu.js"></SCRIPT>
		<LINK href="Common/CSS/Default.CSS" type="text/css" rel="stylesheet">
		<LINK href="Common/CSS/menuStyleXP.CSS" type="text/css" rel="stylesheet">
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<uc1:agenttop id="agenttop1" runat="server"></uc1:agenttop>
			<table id="Table1" cellSpacing="1" cellPadding="3" width="780" border="0">
				<tr>
					<td vAlign="top" width="207" rowSpan="2">
						<table cellSpacing="0" cellPadding="0" width="100%" align="center" border="0">
							<tr bgColor="#bcbcbc">
								<td>
									<table cellSpacing="1" cellPadding="10" width="100%" border="0">
										<tr bgColor="#efefef">
											<td vAlign="top" height="498"><uc1:agentmenu id="agentmenu1" runat="server"></uc1:agentmenu></td>
										</tr>
										<tr bgColor="#f9f9f9">
											<td height="80">
												<div align="center">
													<p><font color="#ff6633">
															<asp:Button id="btnlogout" runat="server" Text="退出後台"></asp:Button></font></p>
												</div>
											</td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</td>
					<td vAlign="top" width="556">
						<table id="Table2" cellSpacing="0" cellPadding="0" width="563" align="center" border="0">
							<tr bgColor="#bcbcbc">
								<td>
									<table id="Table3" cellSpacing="1" cellPadding="8" width="100%" border="0">
										<tr bgColor="#efefef">
											<td vAlign="top" align="left" height="350">
												<TABLE id="Table4" cellSpacing="1" cellPadding="1" width="464" border="0" style="WIDTH: 464px; HEIGHT: 332px">
													<TBODY>
														<TR>
															<TD style="WIDTH: 20%; HEIGHT: 23px">
																<b>帳號信息</b>
															<TD><FONT face="宋体"></FONT></TD>
														<TR>
															<TD style="WIDTH: 163px; HEIGHT: 23px">UserId</TD>
															<TD style="HEIGHT: 23px"><%=Session["agent_id"]%><FONT face="宋体"></FONT></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 163px; HEIGHT: 20px">
																現有金幣</TD>
															<TD style="HEIGHT: 20px"><%=gold%><FONT face="宋体"></FONT></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 163px"><FONT face="宋体"></FONT></TD>
															<TD style="WIDTH: 107px">
																<asp:Button id="btnedit" runat="server" Text="修 改 配 置" Visible="False"></asp:Button></TD>
														</TR>
													</TBODY>
												</TABLE>
												</FONT>
											</td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</td>
				</tr>
			</table>
		</form>
	</body>
</HTML>
