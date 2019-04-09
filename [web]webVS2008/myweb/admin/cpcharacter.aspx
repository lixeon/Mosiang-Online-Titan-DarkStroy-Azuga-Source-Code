<%@ Page language="c#" Codebehind="cpcharacter.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpcharacter" %>
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
														<TD style="WIDTH: 634px; HEIGHT: 22px" colSpan="2"><b>角色信息查詢</b></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2">
															<table style="WIDTH: 464px; HEIGHT: 171px" width="464" border="0">
																<tr>
																	<td style="WIDTH: 75px">帳號</td>
																	<td><asp:textbox id="tbsuserid" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<tr>
																	<td style="WIDTH: 75px">角色名</td>
																	<td><asp:textbox id="tbschaname" runat="server"></asp:textbox>(模糊匹配)</td>
																</tr>
																<TR>
																	<TD style="WIDTH: 75px; HEIGHT: 17px">角色等級</TD>
																	<TD style="HEIGHT: 17px"><asp:textbox id="tbschalvmin" runat="server" Width="64px">0</asp:textbox>至
																		<asp:textbox id="tbschalvmax" runat="server" Width="64px">0</asp:textbox>之間</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 75px">四項屬性合計</TD>
																	<TD><asp:textbox id="tbschapointmin" runat="server" Width="152px">0</asp:textbox>至
																		<asp:textbox id="tbschapointmax" runat="server" Width="152px">0</asp:textbox>之間</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 75px">轉生數</TD>
																	<TD><asp:textbox id="tbscharesetmin" runat="server" Width="104px">0</asp:textbox>至
																		<asp:textbox id="tbscharesetmax" runat="server" Width="96px">0</asp:textbox>之間</TD>
																</TR>
																<TR>
																	<TD style="WIDTH: 75px"><FONT face="宋体"></FONT></TD>
																	<TD><asp:button id="btnsearch" runat="server" Text="查詢角色信息"></asp:button>注：全部留空表示查所有的，效率會很低</TD>
																</TR>
															</table>
														</TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><B>角色信息列表(共有角色<%=rcount%>
																個)</B></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 22px" colSpan="2"><asp:datagrid id="DataGrid1" runat="server" AutoGenerateColumns="False" AllowPaging="True">
																<Columns>
																	<asp:BoundColumn DataField="character_idx" HeaderText="角色IDX"></asp:BoundColumn>
																	<asp:BoundColumn DataField="user_idx" HeaderText="帳號IDX"></asp:BoundColumn>
																	<asp:BoundColumn DataField="character_name" HeaderText="角色名"></asp:BoundColumn>
																	<asp:BoundColumn DataField="character_gender" HeaderText="性別"></asp:BoundColumn>
																	<asp:BoundColumn DataField="character_grade" HeaderText="等級"></asp:BoundColumn>
																	<asp:BoundColumn DataField="CHARACTER_PLAYTIME" HeaderText="遊戲時長(秒)"></asp:BoundColumn>
																	<asp:BoundColumn DataField="webchareset" HeaderText="轉生次數"></asp:BoundColumn>
																	<asp:BoundColumn DataField="CHARACTER_LASTMODIFIED" HeaderText="退出日期"></asp:BoundColumn>
																	<asp:ButtonColumn Text="選擇" ButtonType="PushButton" HeaderText="選擇" CommandName="Select"></asp:ButtonColumn>
																	<asp:ButtonColumn Text="&lt;div id=&quot;de&quot; onclick=&quot;JavaScript:return confirm('確定刪除嗎?')&quot;&gt;刪除&lt;/div&gt;"
																		HeaderText="操作" CommandName="Delete"></asp:ButtonColumn>
																</Columns>
																<PagerStyle Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 182px; HEIGHT: 22px" colSpan="2"><B>角色詳細信息</B></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">帳號IDX</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbuseridx" runat="server" Width="120px" ReadOnly="True" BackColor="#E0E0E0"></asp:textbox><asp:button id="btnsearuseridx" runat="server" Text="查帳號信息"></asp:button><FONT face="宋体">&nbsp;</FONT><FONT face="宋体">&nbsp;</FONT></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">角色IDX</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbchaidx" runat="server" Width="120px" ReadOnly="True" BackColor="#E0E0E0"></asp:textbox><asp:button id="btnsearchitem" runat="server" Text="角色物品欄"></asp:button><FONT face="宋体">&nbsp;
																<asp:button id="btnsearchst" runat="server" Text="普通倉庫"></asp:button>&nbsp;</FONT>
															<asp:button id="btnsearchbbst" runat="server" Text="百寶倉庫"></asp:button><FONT face="宋体">&nbsp;</FONT><FONT face="宋体"></FONT></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">角色名</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbchaname" runat="server" Width="120px"></asp:textbox><asp:button id="btnsearchskill" runat="server" Text="角色武功清單"></asp:button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">所屬幫會</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbguild" runat="server" Width="88px" ReadOnly="True" BackColor="#E0E0E0"></asp:textbox><asp:button id="btnsearchguild" runat="server" Text="幫會信息" Enabled="False"></asp:button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">隊伍IDX</TD>
														<TD style="WIDTH: 636px; HEIGHT: 22px"><asp:textbox id="tbparty" runat="server" Width="88px" ReadOnly="True" BackColor="#E0E0E0"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 25px">性別</TD>
														<TD style="WIDTH: 636px; HEIGHT: 25px"><asp:dropdownlist id="ddsex" runat="server">
																<asp:ListItem Value="0">男</asp:ListItem>
																<asp:ListItem Value="1">女</asp:ListItem>
															</asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 17px">職業</TD>
														<TD style="WIDTH: 55px; HEIGHT: 17px"><asp:dropdownlist id="ddjob" runat="server" Enabled="False">
																<asp:ListItem Value="暫無用">暫無用</asp:ListItem>
															</asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 17px">化境/極魔</TD>
														<TD style="WIDTH: 55px; HEIGHT: 17px"><asp:dropdownlist id="ddstage" runat="server">
																<asp:ListItem Value="0">普通</asp:ListItem>
																<asp:ListItem Value="64">化境</asp:ListItem>
																<asp:ListItem Value="128">極魔</asp:ListItem>
																<asp:ListItem Value="65">玄境</asp:ListItem>
																<asp:ListItem Value="129">脫魔</asp:ListItem>
	
															</asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 9px">登陸地圖</TD>
														<TD style="WIDTH: 55px; HEIGHT: 9px"><asp:dropdownlist id="ddmap" runat="server"></asp:dropdownlist></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">等級</TD>
														<TD><asp:textbox id="tbchalv" runat="server" Width="56px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">遊戲幣</TD>
														<TD><asp:textbox id="tbmoney" runat="server" Width="152px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">轉生次數</TD>
														<TD style="WIDTH: 55px; HEIGHT: 22px"><asp:textbox id="tbchareset" runat="server" Width="80px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 22px">改名次數</TD>
														<TD style="WIDTH: 55px; HEIGHT: 22px"><asp:textbox id="tbnewname" runat="server" Width="80px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">力量</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbgengoal" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">敏捷</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbdex" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">體質</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbsta" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 3px">心胍</TD>
														<TD style="WIDTH: 55px; HEIGHT: 3px"><asp:textbox id="tbsimmak" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">剩余點</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbpoint" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">技能點</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbabilityexp" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 24px">惡名</TD>
														<TD style="WIDTH: 55px; HEIGHT: 24px"><asp:textbox id="tbbadfame" runat="server" Width="128px"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 19px">退出日期</TD>
														<TD style="WIDTH: 55px; HEIGHT: 19px"><asp:textbox id="tblogouttime" runat="server" ReadOnly="True" Enabled="False" BackColor="#E0E0E0"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 19px">總遊戲時間</TD>
														<TD style="WIDTH: 55px; HEIGHT: 19px"><asp:textbox id="tbplaytime" runat="server"></asp:textbox></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 84px; HEIGHT: 27px"></TD>
														<TD style="WIDTH: 55px; HEIGHT: 27px"><asp:button id="btnedit" runat="server" Text="修改角色信息"></asp:button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px" vAlign="top" colSpan="2"></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 636px; HEIGHT: 19px" align="left" colSpan="2"></TD>
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
