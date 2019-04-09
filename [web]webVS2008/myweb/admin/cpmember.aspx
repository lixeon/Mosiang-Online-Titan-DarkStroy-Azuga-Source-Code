<%@ Page language="c#" Codebehind="cpmember.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpmember" %>
<%@ Register TagPrefix="uc1" TagName="cpmenu" Src="../control/cpmenu.ascx" %>
<%@ Register TagPrefix="uc1" TagName="cptop" Src="../control/cptop.ascx" %>
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
												</div>
												</FONT></FONT></td>
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
														<TD style="WIDTH: 634px; HEIGHT: 22px" colSpan="2"><b>帳號信息查詢</b></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2">
															<table style="WIDTH: 464px; HEIGHT: 171px" width="464" border="0">
																<TR>
																	<TD style="WIDTH: 60px">帳號IDX</TD>
																	<TD>
																		<asp:textbox id="tbsuseridx" runat="server">0</asp:textbox></TD>
																</TR>
																<tr>
																	<td style="WIDTH: 60px">帳號</td>
																	<td><asp:textbox id="tbsuserid" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">密碼</td>
																	<td><asp:textbox id="tbspassword" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">超級密碼</td>
																	<td><asp:textbox id="tbskey" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">IP</td>
																	<td><asp:textbox id="tbswebregip" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">EMAIL</td>
																	<td><asp:textbox id="tbsemail" runat="server" Width="256px"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px">角色名</td>
																	<td><asp:textbox id="tbschaname" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 60px; HEIGHT: 12px">會員等級</td>
																	<td style="HEIGHT: 12px"><asp:dropdownlist id="ddsweblevel" runat="server">
																			<asp:ListItem Value="9999" Selected="True">未選擇</asp:ListItem>
																			<asp:ListItem Value="0">普通會員</asp:ListItem>
																			<asp:ListItem Value="1">黃金會員</asp:ListItem>
																			<asp:ListItem Value="2">白金會員</asp:ListItem>
																			<asp:ListItem Value="3">鑽石會員</asp:ListItem>
																		</asp:dropdownlist></td>
																</tr>
																<TR>
																	<TD style="WIDTH: 60px">網銀金額</TD>
																	<TD><asp:textbox id="tbswebbankmin" runat="server" Width="152px">0</asp:textbox>至
																		<asp:textbox id="tbswebbankmax" runat="server" Width="152px">0</asp:textbox>之間</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 60px">金幣</TD>
																	<TD><asp:textbox id="tbswebgoldmin" runat="server" Width="152px">0</asp:textbox>至
																		<asp:textbox id="tbswebgoldmax" runat="server" Width="152px">0</asp:textbox>之間</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 60px; HEIGHT: 26px">註冊時間</TD>
																	<TD style="HEIGHT: 26px"><asp:textbox id="tbscreatetimestart" runat="server" Width="104px"></asp:textbox>至
																		<asp:textbox id="tbscreatetimeend" runat="server" Width="96px"></asp:textbox>之間(日期格式為2007-01-01)</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 60px"></TD>
																	<TD><asp:button id="btnsearch" runat="server" Text="查詢帳號信息"></asp:button>注：全部留空表示查所有的，效率會很低</TD>
																</TR>
															</table>
														</TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><B>帳號信息列表(共有帳號<%=rcount%>條)</B></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><asp:datagrid id="DataGrid1" runat="server" AllowPaging="True" AutoGenerateColumns="False" Width="536px">
																<Columns>
																	<asp:BoundColumn DataField="user_idx" HeaderText="帳號編碼"></asp:BoundColumn>
																	<asp:BoundColumn DataField="user_id" HeaderText="帳號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="webemail" HeaderText="EMAIL"></asp:BoundColumn>
																	<asp:BoundColumn DataField="webregIP" HeaderText="註冊IP"></asp:BoundColumn>
																	<asp:BoundColumn DataField="webgold" HeaderText="金幣"></asp:BoundColumn>
																	<asp:BoundColumn DataField="create_time" HeaderText="註冊時間"></asp:BoundColumn>
																	<asp:ButtonColumn Text="選擇" ButtonType="PushButton" HeaderText="選擇" CommandName="Select"></asp:ButtonColumn>
																</Columns>
																<PagerStyle Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 182px; HEIGHT: 22px" colSpan="2"><B>帳號詳細信息</B></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">帳號</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbuserid" runat="server" Width="104px" ReadOnly="True" BackColor="#E0E0E0"></asp:textbox>
															<asp:Button id="btnsearchcha" runat="server" Text="查此帳號所有角色"></asp:Button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">密碼</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbpassword" runat="server" Width="176px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">二級密碼</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbwebkey" runat="server" Width="176px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">註冊郵箱</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbwebemail" runat="server" Width="232px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">性別</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:dropdownlist id="ddsex" runat="server" Enabled="False">
																<asp:ListItem Value="帥哥">男</asp:ListItem>
																<asp:ListItem Value="美女">女</asp:ListItem>
															</asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 16px">VIP</TD>
														<TD style="WIDTH: 55px; HEIGHT: 16px"><asp:dropdownlist id="ddvip" runat="server">
																<asp:ListItem Value="0">普通會員</asp:ListItem>
																<asp:ListItem Value="1">黃金會員</asp:ListItem>
																<asp:ListItem Value="2">白金會員</asp:ListItem>
																<asp:ListItem Value="3">鑽石會員</asp:ListItem>
															</asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 16px">等級</TD>
														<TD style="WIDTH: 55px; HEIGHT: 16px">
															<asp:dropdownlist id="ddlevel" runat="server">
																<asp:ListItem Value="1">神</asp:ListItem>
																<asp:ListItem Value="2">程序員</asp:ListItem>
																<asp:ListItem Value="3">開發者</asp:ListItem>
																<asp:ListItem Value="4">GM</asp:ListItem>
																<asp:ListItem Value="5">超級玩家</asp:ListItem>
																<asp:ListItem Value="6">普通玩家</asp:ListItem>
																<asp:ListItem Value="10">阻止用戶</asp:ListItem>
															</asp:dropdownlist></FONT></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">金幣</TD>
														<TD><asp:textbox id="tbwebgold" runat="server" Width="88px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 22px">網銀</TD>
														<TD style="WIDTH: 55px; HEIGHT: 22px"><asp:textbox id="tbwebbank" runat="server"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 26px">註冊IP</TD>
														<TD style="WIDTH: 55px; HEIGHT: 26px"><asp:textbox id="tbwebregip" runat="server" ReadOnly="True" Enabled="False" BackColor="#E0E0E0"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 19px">創建日期</TD>
														<TD style="WIDTH: 55px; HEIGHT: 19px"><asp:textbox id="tbcreatetime" runat="server" ReadOnly="True" Enabled="False"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 74px; HEIGHT: 27px"></TD>
														<TD style="WIDTH: 55px; HEIGHT: 27px"><asp:button id="btnedit" runat="server" Text="修改帳號信息"></asp:button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px" vAlign="top" colSpan="2"></TD>
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
