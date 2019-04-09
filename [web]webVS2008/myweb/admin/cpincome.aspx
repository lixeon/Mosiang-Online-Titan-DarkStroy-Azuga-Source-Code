<%@ Page language="c#" Codebehind="cpincome.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpincome" %>
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
													<p><font color="#ff6633"><A href="../logout.aspx" target="_blank"><b><font color="#ff6666">註銷退出</b></A></p>
												</div>
												</FONT></FONT></td>
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
											<td vAlign="top" align="left">
												<TABLE id="Table4" style="WIDTH: 560px" cellSpacing="1" cellPadding="1" width="560" border="0">
													<TR>
														<TD style="WIDTH: 107px; HEIGHT: 36px" colSpan="2"><b>三日內贊助排行</b></TD>
													</TR>
													<TR>
														<TD colSpan="2"><FONT face="宋体">
																<asp:datagrid id="DataGrid1" runat="server" PageSize="50" Width="552px" AutoGenerateColumns="False"
																	BorderColor="#CCCCCC" BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None"
																	AllowPaging="True">
																	<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																	<ItemStyle ForeColor="#000066"></ItemStyle>
																	<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																	<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																	<Columns>
																		<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
																		<asp:BoundColumn DataField="playerid" HeaderText="遊戲帳號"></asp:BoundColumn>
																		<asp:BoundColumn DataField="gold" HeaderText="金幣"></asp:BoundColumn>
																	</Columns>
																	<PagerStyle Visible="False" HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
																</asp:datagrid></FONT></TD>
													</TR>
												</TABLE>
												<TABLE id="Table5" style="WIDTH: 560px" cellSpacing="1" cellPadding="1" width="560" border="0">
													<TR>
														<TD style="WIDTH: 107px; HEIGHT: 36px" colSpan="2"><B>四周排行&nbsp;</B></TD>
													</TR>
													<TR>
														<TD colSpan="2">
															<asp:datagrid id="DataGrid2" runat="server" Width="552px" AutoGenerateColumns="False" BorderColor="#CCCCCC"
																BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None" PageSize="50" AllowPaging="True">
																<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																<ItemStyle ForeColor="#000066"></ItemStyle>
																<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																<Columns>
																	<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
																	<asp:BoundColumn DataField="playerid" HeaderText="玩家帳號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="gold" HeaderText="金幣"></asp:BoundColumn>
																</Columns>
																<PagerStyle Visible="False" HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
													</TR>
												</TABLE>
												<TABLE id="Table5" style="WIDTH: 560px" cellSpacing="1" cellPadding="1" width="560" border="0">
													<TR>
														<TD style="WIDTH: 107px; HEIGHT: 36px" colSpan="2"><B>本月排行</B></TD>
													</TR>
													<TR>
														<TD colSpan="2">
															<asp:datagrid id="DataGrid3" runat="server" Width="552px" AutoGenerateColumns="False" BorderColor="#CCCCCC"
																BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None" PageSize="50" AllowPaging="True">
																<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																<ItemStyle ForeColor="#000066"></ItemStyle>
																<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																<Columns>
																	<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
																	<asp:BoundColumn DataField="playerid" HeaderText="玩家帳號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="gold" HeaderText="金幣"></asp:BoundColumn>
																</Columns>
																<PagerStyle Visible="False" HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
													</TR>
												</TABLE>
												<P><FONT face="宋体"></FONT></P>
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
