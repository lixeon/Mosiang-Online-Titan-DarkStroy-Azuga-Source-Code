namespace web.agent
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class gold : Page
    {
        protected Button btnedit;
        protected Button btnlogout;
        protected TextBox tbgold;
        protected TextBox tbuserid;

        private void btnedit_Click(object sender, EventArgs e)
        {
            int gold = Math.Abs(int.Parse(this.tbgold.Text.ToString()));
            string playerid = new system().ChkSql(this.tbuserid.Text.ToString());
            string clientIP = new system().GetClientIP();
            string agentid = this.Session["agent_id"].ToString();
            string str4 = new WebLogic().agentsellgold(agentid, playerid, clientIP, gold);
            base.Response.Write("<script language=javascript>alert('" + str4 + "')</script>");
        }

        private void btnlogout_Click(object sender, EventArgs e)
        {
            this.Session.Abandon();
            base.Response.Redirect("default.aspx");
        }

        private void InitializeComponent()
        {
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
            this.btnlogout.Click += new EventHandler(this.btnlogout_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isagent();
        }
    }
}
