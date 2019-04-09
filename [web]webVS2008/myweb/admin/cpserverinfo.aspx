<%@ Page language="c#" Codebehind="cpserverinfo.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpserverinfo" %>
<%@ Register TagPrefix="uc1" TagName="cptop" Src="../control/cptop.ascx" %>
<%@ Register TagPrefix="uc1" TagName="cpmenu" Src="../control/cpmenu.ascx" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>admincp</title>
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
			<uc1:cptop id="Cptop1" runat="server"></uc1:cptop>
			<table id="Table1" cellSpacing="1" cellPadding="3" width="780" border="0">
				<tr>
					<td vAlign="top" width="207" rowSpan="2">
						<table cellSpacing="0" cellPadding="0" width="100%" align="center" border="0">
							<tr bgColor="#bcbcbc">
								<td>
									<table cellSpacing="1" cellPadding="10" width="100%" border="0">
										<tr bgColor="#efefef">
											<td vAlign="top" height="498"><uc1:cpmenu id="Cpmenu1" runat="server"></uc1:cpmenu></td>
										</tr>
										<tr bgColor="#f9f9f9">
											<td height="80">
												<div align="center">
													<p><font color="#ff6633"><A href="../logout.aspx" target="_blank"><b><font color="#ff6666">註銷退出</font></b></A></font></p>
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
												<TABLE id="Table4" cellSpacing="1" cellPadding="1" width="440" border="0" style="WIDTH: 440px; HEIGHT: 328px">
													<TR>
														<TD colSpan="2"><b>服務器信息</b></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 30%">在線玩家人數</TD>
														<TD style="HEIGHT: 32px">
															<asp:Label id="lblonlineplayer" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">註冊總人數</TD>
														<TD>
															<asp:Label id="lblregister" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">男玩家人數</TD>
														<TD>
															<asp:Label id="lblmale" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">女玩家人數</TD>
														<TD>
															<asp:Label id="lblfemale" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">總角色數量</TD>
														<TD>
															<asp:Label id="lblallcharacter" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">VIP玩家數量</TD>
														<TD>
															<asp:Label id="lblvip" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">2級寵數量</TD>
														<TD>
															<asp:Label id="lbl2lvpet" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">3級寵數量</TD>
														<TD>
															<asp:Label id="lbl3lvpet" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px">網銀存款總數</TD>
														<TD>
															<asp:Label id="lblbank" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 112px">玩家金幣總量</TD>
														<TD>
															<asp:Label id="lblplayergold" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 112px">代理金幣總量</TD>
														<TD>
															<asp:Label id="lblagentgold" runat="server"></asp:Label></TD>
													</TR>
													<TR>
														<TD colspan="2" align="center">
															<asp:Button id="btnread" runat="server" Text="讀取所有信息"></asp:Button>
															<asp:Button id="btnreadonline" runat="server" Text="只讀取在線人數"></asp:Button></TD>
													</TR>
												</TABLE>
												</FONT></td>
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
