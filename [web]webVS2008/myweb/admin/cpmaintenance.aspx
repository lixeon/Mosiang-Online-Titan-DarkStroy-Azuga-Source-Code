<%@ Register TagPrefix="uc1" TagName="cpmenu" Src="../control/cpmenu.ascx" %>
<%@ Register TagPrefix="uc1" TagName="cptop" Src="../control/cptop.ascx" %>
<%@ Page language="c#" Codebehind="cpmaintenance.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpmaintenance" %>
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
												<TABLE id="Table4" cellSpacing="1" cellPadding="1" width="440" border="0" style="WIDTH: 440px; HEIGHT: 0px">
													<TR>
														<TD colSpan="2" style="HEIGHT: 23px"><b>遊戲維護</b></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 30%">重建索引</TD>
														<TD style="HEIGHT: 32px">
															<asp:Button id="btnmaintenance" runat="server" Text="一天只能執行一次,需要時間比較長"></asp:Button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px; HEIGHT: 27px">更新會員(代理)</TD>
														<TD style="HEIGHT: 27px">
															<asp:Button id="btnupdatemember" runat="server" Text="根據贊助情況更新會員信息"></asp:Button></TD>
													</TR>
													<TR>
														<TD style="WIDTH: 110px; HEIGHT: 27px">更新會員(支付寶)</TD>
														<TD style="HEIGHT: 27px">
															<asp:Button id="btnupdatememberbyalipay" runat="server" Text="根據贊助情況更新會員信息"></asp:Button></TD>
													</TR>
													<TR>
														<TD colspan="2" align="center"><FONT face="宋体"></FONT></TD>
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
