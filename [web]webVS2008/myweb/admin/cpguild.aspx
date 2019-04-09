<%@ Page language="c#" Codebehind="cpguild.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpguild" %>
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
			<table id="Table1" style="WIDTH: 803px; HEIGHT: 780px" cellSpacing="1" cellPadding="3"
				width="803" border="0">
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
													<p><font color="#ff6633"><A href="../logout.aspx" target="_blank"><b><font color="#ff6666">註銷退出</b></A></p>
												</div></FONT></FONT></td>
										</tr>
									</table>
								</td>
							</tr>
						</table>
					</td>
					<td vAlign="top" width="556">
						<table id="Table2" style="WIDTH: 550px; HEIGHT: 756px" cellSpacing="0" cellPadding="0"
							width="550" align="center" border="0">
							<tr bgColor="#bcbcbc">
								<td>
									<table id="Table3" cellSpacing="1" cellPadding="8" width="100%" border="0">
										<tr bgColor="#efefef">
											<td vAlign="top" align="left">
												<TABLE id="Table4" style="WIDTH: 559px; HEIGHT: 768px" cellSpacing="1" cellPadding="1"
													width="559" border="0">
													<TR>
														<TD style="WIDTH: 634px; HEIGHT: 22px" colSpan="2"><b>幫會信息查詢</b></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2">
															<table style="WIDTH: 464px; HEIGHT: 96px" width="464" border="0">
																<tr>
																	<td style="WIDTH: 60px">幫會名</td>
																	<td><asp:textbox id="tbguildname" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">幫主名</td>
																	<td><asp:textbox id="tbmastername" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<TR>
																	<TD style="WIDTH: 60px"><FONT 
                              face=宋体></FONT></TD>
																	<TD><asp:button id="btnsearch" runat="server" Text="查詢幫會資訊"></asp:button>注：全部留空表示查所有的，效率會很低</TD>
																</TR>
															</table>
														</TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><B>幫會信息列表(共有幫會 
                        個)</B></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><asp:datagrid id="DataGrid1" runat="server" AllowPaging="True" AutoGenerateColumns="False" Width="536px" Height="272px">
<Columns>
<asp:BoundColumn DataField="guildidx" HeaderText="幫會編碼"></asp:BoundColumn>
<asp:BoundColumn DataField="guildname" HeaderText="幫會名"></asp:BoundColumn>
<asp:BoundColumn DataField="mastername" HeaderText="幫主名"></asp:BoundColumn>
<asp:BoundColumn DataField="guildlevel" HeaderText="幫會等級"></asp:BoundColumn>
</Columns>

<PagerStyle Mode="NumericPages">
</PagerStyle>
															</asp:datagrid></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px" vAlign="top" colSpan="2"><FONT face=宋体></FONT></TD>
													</TR>
													<TR>
														<TD align="left" colSpan="2" style="WIDTH: 636px; HEIGHT: 19px"><FONT face="宋体"></FONT>
														</TD>
													</TR>
												</TABLE>
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
