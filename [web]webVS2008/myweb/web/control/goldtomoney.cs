namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class goldtomoney : UserControl
    {
        protected Button btnchange;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected TextBox tbgold;

        private void btnchange_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int gold = Math.Abs(int.Parse(this.tbgold.Text.ToString()));
            int goldtomoney = int.Parse(base.Application["game.goldtomoney"].ToString());
            string str = new WebLogic().goldtomoney(useridx, gold, goldtomoney);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnchange.Click += new EventHandler(this.btnchange_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
        }
    }
}
