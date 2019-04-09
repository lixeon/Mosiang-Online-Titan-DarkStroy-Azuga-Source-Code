<%@ Page language="c#" Codebehind="cpfunset.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpfunset" %>
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
												<TABLE id="Table4" cellSpacing="1" cellPadding="1" width="468" border="0" style="WIDTH: 468px; HEIGHT: 134px">
													<TBODY>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 23px">
																<b>遊戲參數配置</b>
															<TD></TD>
														<TR>
															<TD width="123" style="WIDTH: 123px">可改名次數</TD>
															<TD width="75%">
																<asp:TextBox id="tbmodifychanamecount" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD width="123" style="WIDTH: 123px">改名需要遊戲幣</TD>
															<TD width="75%">
																<asp:TextBox id="tbmodifychanamemoney" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 20px">
																轉生次數</TD>
															<TD style="HEIGHT: 20px">
																<asp:TextBox id="tbcharesetcount" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 20px">
																轉生送點</TD>
															<TD style="HEIGHT: 20px">
																<asp:TextBox id="tbcharesetgivepoint" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 26px">
																第一轉所需等級</TD>
															<TD style="HEIGHT: 26px">
																<asp:TextBox id="tbcharesetflv" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 20px">
																後續每轉增加等級</TD>
															<TD style="HEIGHT: 20px">
																<asp:TextBox id="tbcharesetlvstep" runat="server" Width="84px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 20px">
																第一轉需要遊戲幣</TD>
															<TD style="HEIGHT: 20px">
																<asp:TextBox id="tbcharesetfmoney" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 20px">
																後續每轉增加遊戲幣</TD>
															<TD style="HEIGHT: 20px">
																<asp:TextBox id="tbcharesetmoneystep" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">推薦得錢</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbcommendmoney" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																推薦得金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbcommendgold" runat="server" Width="132px"></asp:TextBox></FONT></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																推薦廣告語</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbcommendtext" runat="server" Width="348px" TextMode="MultiLine" Height="134px"></asp:TextBox></FONT></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																洗紅名需要金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbclearpkgold" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																升級2級寵物需金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbpetlvupgold2" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																升級3級寵物需金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbpetlvupgold3" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																升級技能需遊戲幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbskilllvupmoney" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																升級物品所需金币</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbitemlvupgold" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																升級物品機率</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbitemlvuprate" runat="server" Width="132px"></asp:TextBox>*數值越大越難成功</TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px; HEIGHT: 26px">
																金幣換遊戲幣</TD>
															<TD style="HEIGHT: 26px">
																<asp:TextBox id="tbgoldtomoney" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																洗點需遊戲幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbresetpointmoney" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																洗點需金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbresetpointgold" runat="server" Width="132px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																會員購物打折</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbvipoffer" runat="server" Width="80px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																競拍報名費用</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbpssigngold" runat="server" Width="80px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px">
																修改信息金幣</TD>
															<TD style="HEIGHT: 22px">
																<asp:TextBox id="tbchangeinfogold" runat="server" Width="80px"></asp:TextBox></TD>
														</TR>
														<TR>
															<TD style="WIDTH: 123px"></TD>
															<TD style="WIDTH: 107px">
																<asp:Button id="btnedit" runat="server" Text="修 改 配 置"></asp:Button></TD>
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
