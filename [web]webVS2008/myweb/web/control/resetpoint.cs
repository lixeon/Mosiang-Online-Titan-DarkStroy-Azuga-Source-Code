namespace web.control
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class resetpoint : UserControl
    {
        protected Button btnreset;
        protected DropDownList ddchalist;
        protected RequiredFieldValidator Requiredfieldvalidator6;

        private void btnreset_Click(object sender, EventArgs e)
        {
            int useridx = int.Parse(base.Session["useridx"].ToString());
            int chaidx = int.Parse(this.ddchalist.SelectedValue.ToString());
            int resetpointmoney = int.Parse(base.Application["game.resetpointmoney"].ToString());
            int resetpointgold = int.Parse(base.Application["game.resetpointgold"].ToString());
            int charesetgivepoint = int.Parse(base.Application["game.charesetgivepoint"].ToString());
            string str = new WebLogic().resetpoint(base.Session["userid"].ToString(), useridx, chaidx, resetpointmoney, resetpointgold, charesetgivepoint);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void InitializeComponent()
        {
            this.btnreset.Click += new EventHandler(this.btnreset_Click);
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
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
            }
        }
    }
}
