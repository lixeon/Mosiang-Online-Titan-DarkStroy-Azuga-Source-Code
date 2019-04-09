<%@ Page language="c#" Codebehind="cploginlog.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cploginlog" %>
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
												<TABLE id="Table5" style="WIDTH: 560px" cellSpacing="1" cellPadding="1" width="560" border="0">
													<TR>
														<TD style="WIDTH: 107px; HEIGHT: 36px" colSpan="2">
															<P><B></B><B> 查找玩家賬號登陸日誌
																	<TABLE id="Table4" style="WIDTH: 464px; HEIGHT: 171px" width="464" border="0">
																		<TR>
																			<TD style="WIDTH: 60px">帳號</TD>
																			<TD>
																				<asp:textbox id="tbsuserid" runat="server"></asp:textbox>(模糊匹配)</TD>
																		</TR>
																		<TR>
																			<TD style="WIDTH: 60px">IP</TD>
																			<TD>
																				<asp:textbox id="tbswebregip" runat="server"></asp:textbox>(模糊匹配)</TD>
																		</TR>
																		<TR>
																			<TD style="WIDTH: 60px; HEIGHT: 26px">登陸時間</TD>
																			<TD style="HEIGHT: 26px">
																				<asp:textbox id="tbscreatetimestart" runat="server" Width="104px"></asp:textbox>至
																				<asp:textbox id="tbscreatetimeend" runat="server" Width="96px"></asp:textbox>之間(日期格式為2007-01-01)</TD>
																		</TR>
																		<TR>
																			<TD style="WIDTH: 60px"></TD>
																			<TD>
																				<asp:button id="Button1" runat="server" Text="查詢帳號信息"></asp:button>注：全部留空表示查所有的，效率會很低</TD>
																		</TR>
																	</TABLE>
															</P>
															</B></TD>
													</TR>
													<TR>
														<TD colSpan="2">
															<asp:datagrid id="DataGrid2" runat="server" PageSize="1000" AllowPaging="True" Width="552px" AutoGenerateColumns="False"
																BorderColor="#CCCCCC" BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None">
																<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																<ItemStyle ForeColor="#000066"></ItemStyle>
																<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																<Columns>
																	<asp:BoundColumn DataField="login_id" HeaderText="玩家賬號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="start_date" HeaderText="登陸時間"></asp:BoundColumn>
																	<asp:BoundColumn DataField="end_date" HeaderText="退出時間"></asp:BoundColumn>
																	<asp:BoundColumn DataField="ip" HeaderText="IP"></asp:BoundColumn>
																</Columns>
																<PagerStyle HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
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
