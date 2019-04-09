namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpgold : Page
    {
        protected Button btnedit;
        protected CheckBox cblog;
        protected DataGrid DataGrid1;
        protected DropDownList ddagent;
        protected DropDownList ddagentid;
        protected TextBox playerid;
        protected TextBox tbgold;

        private void btnedit_Click(object sender, EventArgs e)
        {
            string agentid = "";
            string mySql = "";
            int gold = int.Parse(this.tbgold.Text.ToString());
            if ((this.playerid.Text.Trim().ToString() == "") || (this.playerid.Text.Trim().ToString() == null))
            {
                agentid = new system().ChkSql(this.ddagent.SelectedValue.ToString());
                if (this.cblog.Checked)
                {
                    new WebLogic().log("", agentid, this.Session["admin_name"].ToString(), gold, "支付金幣", new system().GetClientIP(), "管理員發放金幣");
                }
                mySql = string.Concat(new object[] { "update mhcmember..web_agent set gold = gold+", gold, " where userid='", agentid, "'" });
                new DataProviders().ExecuteSql(mySql);
                base.Response.Write("<script language=javascript>alert('金幣發放成功!')</script>");
            }
            else
            {
                agentid = new system().ChkSql(this.playerid.Text.Trim().ToString());
                mySql = string.Concat(new object[] { "update mhcmember..chr_log_info set webgold = webgold+", gold, " where id_loginid='", agentid, "'" });
                new DataProviders().ExecuteSql(mySql);
                base.Response.Write("<script language=javascript>alert('玩家金币发放成功!')</script>");
            }
        }

        private void ddagentid_SelectedIndexChanged(object sender, EventArgs e)
        {
            string str = this.ddagentid.SelectedValue.ToString();
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_log where type='管理員發放金幣' and agentid='" + str + "' order by date desc", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void InitializeComponent()
        {
            this.ddagentid.SelectedIndexChanged += new EventHandler(this.ddagentid_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_log where type='管理員發放金幣' order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
                this.ddagent.DataSource = new DataProviders().ExecuteSqlDs("select userid+'['+convert(varchar,gold)+']' as fulluserid,userid from web_agent", "ddagent");
                this.ddagent.DataTextField = "fulluserid";
                this.ddagent.DataValueField = "userid";
                this.ddagent.DataBind();
                this.ddagentid.DataSource = new DataProviders().ExecuteSqlDs("select * from web_agent", "ddagent");
                this.ddagentid.DataTextField = "userid";
                this.ddagentid.DataValueField = "userid";
                this.ddagentid.DataBind();
                this.ddagentid.Items.Insert(0, new ListItem("請選擇", "0"));
            }
        }
    }
}
