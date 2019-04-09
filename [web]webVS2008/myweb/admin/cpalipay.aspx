<%@ Register TagPrefix="uc1" TagName="cptop" Src="../control/cptop.ascx" %>
<%@ Register TagPrefix="uc1" TagName="cpmenu" Src="../control/cpmenu.ascx" %>
<%@ Page language="c#" Codebehind="cpalipay.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpalipay" %>
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
														<TD colSpan="2"><B>訂單明細
																<asp:TextBox id="tbplayerid" runat="server"></asp:TextBox>
																<asp:Button id="btnsearchuserid" runat="server" Text="查找玩家ID"></asp:Button>
																<asp:Button id="btnclear" runat="server" Text="清除昨天以前的垃圾單"></asp:Button></B></TD>
														<TD style="WIDTH: 107px; HEIGHT: 36px"><FONT face="宋体"></FONT></TD>
													</TR>
													<TR>
														<TD colSpan="2">
															<asp:TextBox id="tbtradeno" runat="server"></asp:TextBox>
															<asp:Button id="btnsearchno" runat="server" Text="查找訂單號"></asp:Button></TD>
														<TD style="WIDTH: 107px; HEIGHT: 36px"><FONT face="宋体"></FONT></TD>
													</TR>
													<TR>
														<TD colSpan="2">
															<asp:datagrid id="DataGrid1" runat="server" PageSize="40" AllowPaging="True" Width="552px" AutoGenerateColumns="False"
																BorderColor="#CCCCCC" BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None">
																<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																<ItemStyle ForeColor="#000066"></ItemStyle>
																<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																<Columns>
																	<asp:BoundColumn DataField="tradeno" HeaderText="訂單號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="userid" HeaderText="玩家ID"></asp:BoundColumn>
																	<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
																	<asp:BoundColumn DataField="money" HeaderText="金額"></asp:BoundColumn>
																	<asp:BoundColumn DataField="state" HeaderText="狀態"></asp:BoundColumn>
																	<asp:ButtonColumn Text="&lt;div id=&quot;de&quot; onclick=&quot;JavaScript:return confirm('確定刪除嗎?')&quot;&gt;刪除&lt;/div&gt;"
																		HeaderText="操作" CommandName="Delete"></asp:ButtonColumn>
																</Columns>
																<PagerStyle HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
														<TD></TD>
													</TR>
												</TABLE>
												<TABLE id="Table6" style="WIDTH: 560px" cellSpacing="1" cellPadding="1" width="560" border="0">
													<TR>
														<TD style="WIDTH: 200px; HEIGHT: 36px" colSpan="2"><b>按日統計成交訂單金幣</b></TD>
													</TR>
													<TR>
														<TD colSpan="2"><FONT face="宋体">
																<asp:datagrid id="DataGrid2" runat="server" PageSize="20" AllowPaging="True" Width="552px" AutoGenerateColumns="False"
																	BorderColor="#CCCCCC" BorderWidth="1px" BackColor="White" CellPadding="3" BorderStyle="None">
																	<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
																	<ItemStyle ForeColor="#000066"></ItemStyle>
																	<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
																	<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
																	<Columns>
																		<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
																		<asp:BoundColumn DataField="money" HeaderText="金額"></asp:BoundColumn>
																	</Columns>
																	<PagerStyle HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
																</asp:datagrid></FONT></TD>
													</TR>
												</TABLE>
												<B>按月統計成交訂單金幣</B>
												<asp:datagrid id="DataGrid3" runat="server" BorderStyle="None" CellPadding="3" BackColor="White"
													BorderWidth="1px" BorderColor="#CCCCCC" AutoGenerateColumns="False" Width="552px" AllowPaging="True"
													PageSize="20">
													<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#669999"></SelectedItemStyle>
													<ItemStyle ForeColor="#000066"></ItemStyle>
													<HeaderStyle Font-Bold="True" ForeColor="White" BackColor="#006699"></HeaderStyle>
													<FooterStyle ForeColor="#000066" BackColor="White"></FooterStyle>
													<Columns>
														<asp:BoundColumn DataField="date" HeaderText="日期"></asp:BoundColumn>
														<asp:BoundColumn DataField="money" HeaderText="金額"></asp:BoundColumn>
													</Columns>
													<PagerStyle HorizontalAlign="Left" ForeColor="#000066" BackColor="White" Mode="NumericPages"></PagerStyle>
												</asp:datagrid>
												<P></P>
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
