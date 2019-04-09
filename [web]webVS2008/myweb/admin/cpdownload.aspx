<%@ Page language="c#" Codebehind="cpdownload.aspx.cs" AutoEventWireup="false" Inherits="web.admin.cpdownload" %>
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
													<p><font color="#ff6633"><A href="../logout.aspx" target="_blank"></font><b><font color="#ff6666">註銷退出</font></b></A></p>
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
														<TD style="WIDTH: 107px; HEIGHT: 22px" colSpan="2"><b>下載明細列表</b></TD>
													</TR>
													<TR>
														<TD colSpan="2"><asp:datagrid id="DataGrid1" runat="server" PageSize="20" AllowPaging="True" Width="552px" AutoGenerateColumns="False">
																<Columns>
																	<asp:BoundColumn DataField="id" HeaderText="序號"></asp:BoundColumn>
																	<asp:BoundColumn DataField="name" HeaderText="名稱"></asp:BoundColumn>
																	<asp:BoundColumn DataField="date" HeaderText="加入日期"></asp:BoundColumn>
																	<asp:BoundColumn DataField="size" HeaderText="文件大小(MB)"></asp:BoundColumn>
																	<asp:ButtonColumn Text="&lt;div id=&quot;de&quot; onclick=&quot;JavaScript:return confirm('確定刪除嗎?')&quot;&gt;刪除&lt;/div&gt;"
																		HeaderText="操作" CommandName="Delete"></asp:ButtonColumn>
																	<asp:ButtonColumn Text="選擇" ButtonType="PushButton" CommandName="Select"></asp:ButtonColumn>
																</Columns>
																<PagerStyle Mode="NumericPages"></PagerStyle>
															</asp:datagrid></TD>
													</TR>
													<TR>
														<TD align="left" colSpan="2" style="HEIGHT: 19px">
															<hr>
														</TD>
													</TR>
													<TR>
														<TD align="left" style="WIDTH: 132px"><B>下載列表操作</B></TD>
														<TD align="left"><FONT face="細明體">
																<asp:Label id="lblid" runat="server"></asp:Label></FONT></TD>
													</TR>
													<TR align="left">
														<TD align="left" style="WIDTH: 132px">文件名稱</TD>
														<TD align="left"><asp:textbox id="tbname" runat="server" Columns="20" Width="320px"></asp:textbox>
															<asp:RequiredFieldValidator id="RequiredFieldValidator1" runat="server" ErrorMessage="*" ControlToValidate="tbname"></asp:RequiredFieldValidator></TD>
													</TR>
													<TR>
														<TD align="left" style="WIDTH: 132px">下載鏈接</TD>
														<TD align="left">
															<asp:textbox id="tblink" runat="server" Columns="30" Width="320px"></asp:textbox>
															<asp:RequiredFieldValidator id="RequiredFieldValidator3" runat="server" ControlToValidate="tblink" ErrorMessage="*"></asp:RequiredFieldValidator></TD>
													</TR>
													<TR>
														<TD align="left" style="WIDTH: 132px">文件大小</TD>
														<TD align="left">
															<asp:textbox id="tbsize" runat="server" Columns="4"></asp:textbox>MB
															<asp:RequiredFieldValidator id="RequiredFieldValidator2" runat="server" ErrorMessage="*" ControlToValidate="tbsize"></asp:RequiredFieldValidator></TD>
													</TR>
													<TR>
														<TD align="left" style="WIDTH: 132px">說明</TD>
														<TD align="left">
															<asp:textbox id="tbcomment" runat="server" Columns="70" Width="412px"></asp:textbox>
															<asp:RequiredFieldValidator id="RequiredFieldValidator4" runat="server" ControlToValidate="tbcomment" ErrorMessage="*"></asp:RequiredFieldValidator></TD>
													</TR>
													<TR>
														<TD align="center" colSpan="2">
															<asp:Button id="btnedit" runat="server" Text="修 改 下 載" Visible="False"></asp:Button>
															<asp:CheckBox id="cbdate" runat="server" Visible="False" Text="修改日期"></asp:CheckBox><asp:button id="btnadd" runat="server" Text="增 加 下 載"></asp:button></TD>
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
